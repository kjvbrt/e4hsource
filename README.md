# EDM4hep RDataSource

Implementing RDataSource to read EDM4hep objects in algorithms creating
additional columns in RDataFrame.


## Few questions

* What is the optimal way to paralelize access to the frame/reader?
  ```
  Currently, for every slot a framereader is created
  ```
* Should/can `std::reference_wrapper` with collection be used instead of
    `podio::BaseCollection`?
* How to synchronize with ROOT I/O, how to respect cluster boundaries?
