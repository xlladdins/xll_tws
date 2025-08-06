# xll_tws

Call Interactive Brokers C++ API from Excel.

IB Gateway is a program that allows you to connect to Interactive Brokers servers
and write programs to automate Trader Workstation. They do not provide a library
that you can link to, they provide header and source files you must incorporate
into your program. Their GitHub [repository](https://github.com/InteractiveBrokers/tws-api)
does not compile out of the box so the headers and source are provided in
the `tws_api` folder. These had to be modified to run on Windows.

The [`EWrapper`](https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html).
class declares all callbacks their server might call as pure virtual functions
so you must implement all of them if you inherit from it. This seemed like
a poor design choice but since they provided the source code this was easy to fix.
I modified [`EWrapper.h`](tws_api/EWrapper.h) to provide a default implementation
```
	#define EWRAPPER_VIRTUAL_IMPL {} // =0
```	
This makes it possible to inherit from `EWrapper` and override only the methods
you are interested in for receiving data from the IB Gateway server.

The [`EClientSocket`](https://interactivebrokers.github.io/tws-api/classIBApi_1_1EClientSocket.html)
class is used to connect to the IB Gateway server and request data. It inherits from
[`EClient`](https://interactivebrokers.github.io/tws-api/classIBApi_1_1EClient.html)
and uses a socket to send requests to the server to call functions implemented by the
`EWrapper` subclass you provide. The constructor of `EClientSocket`
takes a `EWrapper` and a `EReaderSignal` pointer. The `EReaderSignal` is used to 
signal any `EReader` running in a thread that data is available to read from the socket.

`Wrapper` creates an `EReaderOSSignal` and `EClientSocket` 
with pointers to `EWrapper` and `EReaderOSSignal`.
The signal is created for the server to use when requested data becomes available.
The client constructor sets the state to `CS_DISCONNECTED`.

`client.EConnect` is called with host and port information. Connection state
is set to `CS_DISCONNECTED` (again) a socket is created and used to connect to the host.
The `m_fd` member of client is initialized to the socket.

`client.sendConnectRequest` sets the state to `CS_CONNECTING`. A message with the API
version and connection options is prepared and sent to the server. If successful
the state is set to `CS_CONNECTED` and the socket is set to nonblocking (`FIONBIO`).
If the connection is not asynchronous a `EReader` is created from the client and signal pointers
and is registered with the client then
`reader.putMessageToQueue()` is called. 


otherwise state is set to `CS_DISCONNECTED`
and the socket is closed. If 