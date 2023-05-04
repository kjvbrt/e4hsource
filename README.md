# EDM4hep RDataSource

Implementing RDataSource to read EDM4hep objects in algorithms creating
additional columns in RDataFrame.


## Few questions

* What is the optimal way to paralelize access to the frame/reader?
  ```
  Currently, mutex guards get method of frame
  ```
* How to split workload to different threads (more files than thread or vice
    versa)?
  ```
  Currently, 2 threads(slots) are expected, there are two frames.
  ```
* Can `std::reference_wrapper` with collection be used instead of
    `podio::BaseCollection`?
