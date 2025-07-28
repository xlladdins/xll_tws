# xll_tws

Call TWS API from Excel.

The [`EWrapper`](https://interactivebrokers.github.io/tws-api/interfaceIBApi_1_1EWrapper.html)
class implements the callbacks that are requested by calls to 
[`EClient`](https://interactivebrokers.github.io/tws-api/classIBApi_1_1EClient.html)
member functions. The IB Gateway server places returned data in a queue
that are processed by an [`EReader`](https://interactivebrokers.github.io/tws-api/classIBApi_1_1EReader.html).
Each message is processed by the `processMsgs` method of the of the `EReader` and the appropriate
`EWrapper` method is invoked.