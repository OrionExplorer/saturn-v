###SATURN V [![Build Status](https://travis-ci.org/OrionExplorer/saturn-v.png?branch=master)](https://travis-ci.org/OrionExplorer/saturn-v)
######Copyright (C) 2011 - 2013
######Marcin Kelar (marcin.kelar@gmail.com)

Interactive Saturn V simulator.

**Features**:
* Best played with friends (at least one astronaut and one flight controller *for now*)
* Built-in chat
* Separated GUI sections for Apollo Command/Service Module, Apollo Lunar Module and Mission Control Center modules
* Fully playable on mobile devices via browser
* Live flight data

**Technology**:
* Saturn V main computer (server application) uses WebSockets and standard sockets simultaneously
* Control panels (client application) uses HTML5, CSS3, JavaScript and make use of Responsive Design for better user experience on every possible device

**Screenshots**:
* Before launch (from MCC panel view) ![Before launch](http://img404.imageshack.us/img404/2193/seuf.png)
* Countdown resume (from MCC panel view) ![MCC countdown](http://img30.imageshack.us/img30/9098/okz2.png)
* S-IC stage separation (from CSM panel view) ![S-IC staging](http://img12.imageshack.us/img12/4223/vn07.png)
* [More](http://imageshack.us/g/1/10321436/)

**TODO**:
- CSM/LM separation from S-IVB stage
- Separate user accounts with authorization (full CRUD)
- Trans Lunar Injection burn (patched conics along with Hohmann Transfer Orbit)
- Apollo CSM/LM coasting to the Moon
- Entering Moon Orbit
- Apollo CSM and LM separation
- Landing on the Moon


All data used for this simulation is available in Apollo Flight Journals at http://history.nasa.gov/ (NASA) and http://www.braeunig.us/apollo/saturnV.htm (Robert A. Braeunig).  
Client code written with [JSMVC framework](https://github.com/OrionExplorer/js-mvc).  
Server code written with [c-websocket](https://github.com/OrionExplorer/c-websocket).
