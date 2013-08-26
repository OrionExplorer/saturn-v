###saturn-v [![Build Status](https://travis-ci.org/OrionExplorer/saturn-v.png?branch=master)](https://travis-ci.org/OrionExplorer/saturn-v)
######Copyright (C) 2011 - 2013
######Marcin Kelar (marcin.kelar@gmail.com)

Interactive Saturn V simulator.

Client code written with [JSMVC framework](https://github.com/OrionExplorer/js-mvc).  
Server code written with [c-websocket](https://github.com/OrionExplorer/c-websocket).

Features:
* Saturn V main computer (server application) uses WebSockets and standard sockets simultaneously
* Best played with friends (at least one astronaut and one flight controller)
* Built-in chat
* Separated GUI sections for Saturn V, Apollo CSM, Apollo LM and Mission Control modules
* Live flight data

TODO:
- [ ] Separate user accounts with authorization
- [ ] Trans Lunar Injection burn (patched conics along with Hohmann Transfer Orbit)
- [ ] Apollo CSM/LM coasting to the Moon
- [ ] Entering Moon Orbit
- [ ] Apollo CSM and LM separation
- [ ] Landing on the Moon


All data used for this simulation is available in Apollo Flight Journals at http://history.nasa.gov/ (NASA) and http://www.braeunig.us/apollo/saturnV.htm (Robert A. Braeunig).
