#include "EDM4hepSource/EDM4hepSource.hxx"

#include <iostream>

namespace e4hsource {
  /**
   * \brief Construct the EDM4hepSource from the provided file.
   */
  EDM4hepSource::EDM4hepSource(std::string_view fileName) : m_nSlots{1},
                                                            m_fileName{fileName}
  {
    std::cout << "EDM4hepSource: Constructing the source ..." << std::endl;

    m_columnNames.emplace_back("MCParticles");

    m_podioReader.openFile(m_fileName);

    unsigned nEvents = m_podioReader.getEntries("events");
    std::cout << "EDM4hepSource: Found " << nEvents << " events in file: \n"
              << "               " << fileName << std::endl;

    m_rangesAll.emplace_back(std::pair<ULong64_t, ULong64_t>{0, nEvents/2});
    m_rangesAll.emplace_back(std::pair<ULong64_t, ULong64_t>{nEvents/2, nEvents});
    m_rangesAvailable.emplace_back(std::pair<ULong64_t, ULong64_t>{0, nEvents/2});
    m_rangesAvailable.emplace_back(std::pair<ULong64_t, ULong64_t>{nEvents/2, nEvents});
  }


  /**
   * \brief Inform the EDM4hepSource of the desired level of parallelism.
   */
  void
  EDM4hepSource::SetNSlots(unsigned int nSlots) {
    std::cout << "EDM4hepSource: Setting num. of slots to: " << m_nSlots << std::endl;

    m_nSlots = nSlots;

    const auto nColumns = 1;
    // Initialize the entire set of addresses
    m_mcParticleAddresses.resize(nColumns, std::vector<const edm4hep::MCParticleCollection*>(m_nSlots, nullptr));
  }


  /**
   * \brief Inform RDataSource that an event-loop is about to start.
   */
  void
  EDM4hepSource::Initialize() {
    std::cout << "EDM4hepSource: Initializing the source ..." << std::endl;

  }


  /**
   * \brief Retrieve from EDM4hepSource a set of ranges of entries that can be
   *        processed concurrently.
   */
  std::vector<std::pair<ULong64_t, ULong64_t>>
  EDM4hepSource::GetEntryRanges() {
    std::cout << "EDM4hepSource: Getting entry ranges ..." << std::endl;

    std::vector<std::pair<ULong64_t, ULong64_t>> rangesToBeProcessed;
    for (auto& range: m_rangesAvailable) {
      rangesToBeProcessed.emplace_back(
          std::pair<ULong64_t, ULong64_t>{range.first, range.second}
      );
      if (rangesToBeProcessed.size() >= m_nSlots) {
        break;
      }
    }

    if (m_rangesAvailable.size() > m_nSlots) {
      m_rangesAvailable.erase(m_rangesAvailable.begin(),
                              m_rangesAvailable.begin() + m_nSlots);
    } else {
      m_rangesAvailable.erase(m_rangesAvailable.begin(),
                              m_rangesAvailable.end());
    }


    std::cout << "EDM4hepSource: Ranges to be processed:\n";
    for (auto& range: rangesToBeProcessed) {
      std::cout << "               {" << range.first << ", " << range.second
                << "}\n";
    }

    if (m_rangesAvailable.size() > 0) {

      std::cout << "EDM4hepSource: Ranges remaining:\n";
      for (auto& range: m_rangesAvailable) {
        std::cout << "               {" << range.first << ", " << range.second
                  << "}\n";
      }
    } else {
      std::cout << "EDM4hepSource: No more remaining ranges.\n";
    }

    return std::move(rangesToBeProcessed);
  }


  /**
   * \brief Inform EDM4hepSource that a certain thread is about to start working
   *        on a certain range of entries.
   */
  void
  EDM4hepSource::InitSlot(unsigned int slot, ULong64_t firstEntry) {
    std::cout << "EDM4hepSource: Initializing slot: " << slot
              << " with first entry " << firstEntry << std::endl;
  }


  /**
   * \brief Inform EDM4hepSource that a certain thread is about to start working
   *        on a certain entry.
   */
  bool
  EDM4hepSource::SetEntry(unsigned int slot, ULong64_t entry) {
    std::cout << "EDM4hepSource: In slot: " << slot << ", setting entry: "
              << entry << std::endl;

    m_mutex.lock();
    m_frames[slot] = podio::Frame(m_podioReader.readEntry("events", entry));
    m_mcParticles[entry] = m_frames[slot].get("MCParticles");

    m_mcParticleAddresses[0][slot] =
        dynamic_cast<const edm4hep::MCParticleCollection*>(m_mcParticles[entry]);
    m_mutex.unlock();

    std::cout << "Address: " << m_mcParticleAddresses[0][slot] << "\n";
    std::cout << "Coll size: " << m_mcParticleAddresses[0][slot]->size() << "\n";
    if (m_mcParticleAddresses[0][slot]->isValid()) {
      std::cout << "Collection valid\n";
    }

    return true;
  }


  /**
   * \brief Inform EDM4hepSource that a certain thread finished working on a
   *        certain range of entries.
   */
  void
  EDM4hepSource::FinalizeSlot(unsigned int slot) {
    std::cout << "EDM4hepSource: Finalizing slot: " << slot << std::endl;
  }


  /**
   * \brief Inform RDataSource that an event-loop finished.
   */
  void
  EDM4hepSource::Finalize() {
    std::cout << "EDM4hepSource: Finalizing ..." << std::endl;
  }


  /**
   * \brief Type-erased vector of pointers to pointers to column values --- one
   *        per slot 
   */
  Record_t
  EDM4hepSource::GetColumnReadersImpl(std::string_view name,
                                      const std::type_info& typeInfo) {
    std::cout << "EDM4hepSource: Getting column reader implementation for column:\n"
              << "               " << name << "\n               with type: "
              << typeInfo.name() << std::endl;

    Record_t columnReaders(m_nSlots);
    for (size_t slotIndex = 0; slotIndex < m_nSlots; ++slotIndex) {
      std::cout << "               Slot index: " << slotIndex << "\n";
      std::cout << "               Address: "
                << &m_mcParticleAddresses[0][slotIndex]
                << std::endl;
      columnReaders[slotIndex] = (void*) &m_mcParticleAddresses[0][slotIndex];
    }

    return columnReaders;
  }


  /**
   * \brief Returns a reference to the collection of the dataset's column names 
   */
  const std::vector<std::string>&
  EDM4hepSource::GetColumnNames() const {
    std::cout << "EDM4hepSource: Looking for column names" << std::endl;

    return m_columnNames;
  }

  /**
   * \brief Checks if the dataset has a certain column.
   */
  bool
  EDM4hepSource::HasColumn(std::string_view columnName) const {
    std::cout << "EDM4hepSource: Looking for column: " << columnName << std::endl;

    if (columnName == "MCParticles") {
      return true;
    }

    return false;
  }


  /**
   * \brief Type of a column as a string. Required for JITting.
   */
  std::string
  EDM4hepSource::GetTypeName(std::string_view columnName) const {
    std::cout << "EDM4hepSource: Looking for type name of column: "
              << columnName << std::endl;

    if (columnName == "MCParticles") {
      return "edm4hep::MCParticleCollection";
    }

    return "float";
  }
}
