# The HTTP Gateway operates as a proxy to 2 or more Web Servers, whose main purpose is to compare responses provided by those servers. The typical use case is when we want to run a regression testing of a Web application. The incoming connection can be routed to one or more destinations, allowing to trace and debug the data flow between 2 connection endpoints. A connection endpoint can either be a network connection, a message queue or a structured file.
Central to the Gateway, a routing table defines how the data and meta-data will be moved between network connectors. An external client app

Actors.
- Client Application
- Web Servers
- Traffic Analyzers
- Traffic Stores

-reads a route definition
- if route is bi-directional
- data will be pushed async from each connector
- when a buffer is returned by one connector, it is passed to all other connectors


$ cd bin
$ cmake ..
$ cmake --build .
$ ctest
