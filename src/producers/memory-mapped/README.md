IPC producers
=============
1.  This will be our interface for other languages.
2.  This is also a consumer since the "real" producer will be built in another language.
3.  Will only support a single client thread. Clients are responsible for mutexes etc for multi-thread access.
4.  Will use a pair of memory-mapped files for implementation. The following needs to be addressed in the protocol:
5.  The app needs to know when it can re-use the response memory-mapped file reqions. The client implementation will need to communicate this information with **requests**.
6.  The client needs to know when it can re-use the request memory-mapped file regions. The app implementation will need to communicate this information with **responses**.