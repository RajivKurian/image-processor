IPC producers
=============
#This will be our interface for other languages.
#This is also a consumer since the "real" producer will be built in another language.
#Will only support a single client thread. Clients are responsible for mutexes etc for multi-thread access.
#Will use a pair of memory-mapped files for implementation. The following needs to be addressed in the protocol:
##The app needs to know when it can re-use the response memory-mapped file reqions. The client implementation will need to communicate this information with '''requests'''.
##The client needs to know when it can re-use the request memory-mapped file regions. The app implementation will need to communicate this information with '''responses'''.