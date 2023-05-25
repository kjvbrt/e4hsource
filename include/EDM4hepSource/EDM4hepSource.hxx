#ifndef EDM4HEP_SOURCE_H__
#define EDM4HEP_SOURCE_H__

#include <vector>
// #include <iostream>
// #include <functional>
// #include <mutex>

#include <ROOT/RDataSource.hxx>
#include <podio/Frame.h>
#include <podio/ROOTFrameReader.h>

// #include <podio/CollectionBase.h>
#include <edm4hep/MCParticleCollection.h>


namespace e4hsource {
  using Record_t = std::vector<void*>;

  class EDM4hepSource final : public ROOT::RDF::RDataSource {
    public:
      EDM4hepSource(std::string_view fileName);

      void SetNSlots(unsigned int nSlots);

      template<typename T>
      std::vector<T**> GetColumnReaders(std::string_view columnName);

      void Initialize();

      std::vector<std::pair<ULong64_t, ULong64_t>> GetEntryRanges();

      void InitSlot(unsigned int slot, ULong64_t firstEntry);

      bool SetEntry(unsigned int slot, ULong64_t entry);

      void FinalizeSlot(unsigned int slot);

      void Finalize();

      const std::vector<std::string>& GetColumnNames() const;

      bool HasColumn(std::string_view columnName) const;

      std::string GetTypeName(std::string_view columnName) const;

    protected:
      Record_t GetColumnReadersImpl (std::string_view name,
                                     const std::type_info& typeInfo);

      std::string AsString() { return "Edm4hep data source"; };

    private:
      /// Number of slots/threads
      unsigned int m_nSlots;
      /// Input filename
      std::string m_fileName;
      /// Ranges of events available to be processed
      std::vector<std::pair<ULong64_t, ULong64_t>> m_rangesAvailable;
      /// Ranges of events available ever created
      std::vector<std::pair<ULong64_t, ULong64_t>> m_rangesAll;
      /// Column names
      std::vector<std::string> m_columnNames;
      /// MCParticleAddresses, m_mcParticleAddresses[columnIndex][slotIndex]
      std::vector<std::vector<const edm4hep::MCParticleCollection*>> m_mcParticleAddresses;
      /// MCParticles, m_mcParticles[entryIndex]
      std::map<ULong64_t, const podio::CollectionBase*> m_mcParticles;

      /// Root podio reader
      podio::ROOTFrameReader m_podioReader;

      /// Podio frame
      std::map<int, podio::Frame> m_frames;

      /// Mutex
      std::mutex m_mutex;
  };


  /**
   * \brief Retrieve from EDM4hepSource per-thread readers for the desired columns.
   */
  template<typename T>
  std::vector<T**>
  EDM4hepSource::GetColumnReaders(std::string_view columnName) {
    std::cout << "EDM4hepSource: Getting column readers for column: " << columnName << std::endl;

    std::vector<T**> readers;

    return readers;
  }
}

#endif /* EDM4HEP_SOURCE_H__ */
