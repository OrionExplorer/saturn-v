### SATURN V [![Build Status](https://travis-ci.org/OrionExplorer/saturn-v.png?branch=master)](https://travis-ci.org/OrionExplorer/saturn-v)
###### Copyright (C) 2011 - 2020 Marcin Kelar (marcin.kelar@gmail.com)

>The Saturn V (pronounced "Saturn Five") was an American human-rated expendable rocket used by NASA's Apollo and Skylab programs from 1967 until 1973. A multistage liquid-fueled launch vehicle, NASA launched 13 Saturn Vs from the Kennedy Space Center, Florida with no loss of crew or payload. It remains the tallest, heaviest, and most powerful rocket ever brought to operational status and still holds the record for heaviest payload launched and heaviest payload capacity to Low Earth orbit (LEO).

Source: Wikipedia.

**Features**:
* Control Saturn V rocket with Apollo spacecraft on it's journey to the Moon *(work in progress)*
* Manage entire flight as Flight Director from lift-off to splash down *(work in progress)*
* Best played with friends (at least one astronaut and one flight controller *for now*)
* Built-in chat
* Separated GUI sections for Apollo Command/Service Module, Apollo Lunar Module and Mission Control Center modules
* Fully playable on mobile devices via browser

**Technology behind**:
* Saturn V main computer (server application) uses WebSockets and standard sockets simultaneously
* Control panels (client application) uses HTML5, CSS3, JavaScript and make use of Responsive Design for better user experience on every possible device

**_Warning:_** _This is work in progress and flight without auto-pilot is extremely difficult._

**Screenshots**:
_Soon_

**Next milestones**:
- CSM/LM separation from S-IVB stage
- Trans-Lunar/Earth Injection burn (patched conics along with Hohmann Transfer Orbit)
- Deorbit burn (CSM and/or LM)
- Apollo CSM/LM coasting to the Moon/Earth
- Enter Lunar/Earth Orbit
- Apollo CSM and LM separation
- Gravity assist in mind
- Separate user accounts with authorization (full CRUD?)


All data used for this simulation is available in Apollo Flight Journals at http://history.nasa.gov/ (NASA) and http://www.braeunig.us/apollo/saturnV.htm (Robert A. Braeunig).  
Client code written with [JSMVC framework](https://github.com/OrionExplorer/js-mvc).  
Server code written with [c-websocket](https://github.com/OrionExplorer/c-websocket).
