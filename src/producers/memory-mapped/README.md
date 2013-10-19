IPC producers
=============
1.  This will be our interface for other languages.
2.  This is also a consumer since the "real" producer will be built in another language.
3.  Will only support a single client thread. Clients are responsible for mutexes etc for multi-thread access.
4.  Will use a pair of memory-mapped files for implementation.