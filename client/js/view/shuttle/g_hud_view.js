function showEngines() {
    print('<div class="box-1-info">'+
            '<table>'+
            '<caption><strong>VEHICLE</strong></caption>'+
                //_showRecord('<strong>RTG</strong>', '<span id="voyager7_RTGStatus">'+getCurrentRTGStatus()+' %</span>')+
                _showRecord('<strong>TOTAL MASS</strong>', '<span id="voyager7_totalMass">0 KG</span>')+
                _showRecord('<strong>PITCH</strong>', '<span id="voyager7_pitch">0° / 0°</span')+
                //_showRecord('<strong>ROLL</strong>', '<span id="voyager7_roll">0°</span')+
                _showRecord('<strong>ROLL</strong>', '<span id="voyager7_rollAzimuth">90° / 90°</span')+
                _showRecord('<strong>YAW</strong>', '<span id="voyager7_yaw">0°</span')+
                _showRecord('<strong>TOTAL</strong>', '<span id="voyager7_distance">0 M</span>')+
                _showRecord('<strong>ALTITUDE</strong>', '<span id="voyager7_altitude">0 M</span>')+
                _showRecord('<strong>MAIN ENGINE</strong>', '<span id="voyager7_mainEngine">OFF</span>')+
                //_showRecord('<strong>HORIZONTAL</br>ALTITUDE</strong>', '<span id="voyager7_horizontalAltitude">'+getHorizontalAltitudeStatus()+'</span>')+
        '</table>'+
        '</div>');
}

function showDistance() {
    print('<div class="box-1-info">'+
        '<table>'+
            '<caption><strong>DISTANCE</strong></caption>'+
                _showRecord('<strong>TOTAL</strong>', '<span id="voyager7_distance">0 M</span>')+
                _showRecord('<strong>ALTITUDE</strong>', '<span id="voyager7_altitude">0 M</span>')+
               // _showRecord('<strong>DESTINATION</strong>', '<span id="voyager7_destination">'+currentAO.getCurrentDestination()+' ('+Math.round(currentAO.getCurrentDestinationAltitude()/100)/10+' KM)</span>')+
                //_showRecord('<strong>CURRENT<br/>CELESTIAL BODY</strong>', '<span id="voyager7_current_ao">'+currentAO.name+'</span>')+
            '</table>'+
        '</div>');
}

function showVelocityData() {
    print('<div class="box-1-info">'+
        '<table>'+
            '<caption><strong>VELOCITY</strong></caption>'+
                _showRecord('<strong>VELOCITY</strong>', '<span id="voyager7_velocity">0 M/S</span>')+
                _showRecord('<strong>VERTICAL VELOCITY</strong>', '<span id="voyager7_verticalVelocity">0 M/S</span>')+
                _showRecord('<strong>HORIZONTAL VELOCITY</strong>', '<span id="voyager7_horizontalVelocity">0 M/S</span>')+
                _showRecord('<strong>ACCELERATION</strong>', '<span id="voyager7_acceleration">0 M/S</span>')+
                _showRecord('<strong>G-FORCE</strong>', '<span id="voyager7_gForce">0 G</span>')+
                _showRecord('<strong>THRUST</strong>', '<span id="voyager7_thrust">0 %</span>')+
                _showRecord('<strong>MAX Q</strong>', '<span id="voyager7_dynamicPressure">0 KG/M²</span>')+
            '</table>'+
        '</div>');
}

function showS1Component() {
    print('<div class="box-1-info">'+
        '<table>'+
            '<caption><strong>S-IC STAGE</strong></caption>'+
                _showRecord('<strong>FUEL</strong>', '<span id="voyager7_s1_fuel">0 KG</span>')+
                _showRecord('<strong>STATUS</strong>', '<span id="voyager7_s1_status">STAGED</span>')+
                _showRecord('<strong>THRUST</strong>', '<span id="voyager7_s1_thrust">0 N</span>')+
                _showRecord('<strong>BURN TIME</strong>', '<span id="voyager7_s1_burnTime">0 S</span>')+
            '</table>'+
        '</div>');
}

function showS2Component() {
    print('<div class="box-1-info">'+
        '<table>'+
            '<caption><strong>S-II STAGE</strong></caption>'+
                _showRecord('<strong>FUEL</strong>', '<span id="voyager7_s2_fuel">0 KG</span>')+
                _showRecord('<strong>STATUS</strong>', '<span id="voyager7_s2_status">STAGED</span>')+
                _showRecord('<strong>THRUST</strong>', '<span id="voyager7_s2_thrust">0 N</span>')+
                _showRecord('<strong>BURN TIME</strong>', '<span id="voyager7_s2_burnTime">0 S</span>')+
            '</table>'+
        '</div>');
}

function showS3Component() {
    print('<div class="box-1-info">'+
        '<table>'+
            '<caption><strong>S-IVB STAGE</strong></caption>'+
                _showRecord('<strong>FUEL</strong>', '<span id="voyager7_s3_fuel">0 KG</span>')+
                _showRecord('<strong>STATUS</strong>', '<span id="voyager7_s3_status">STAGED</span>')+
                _showRecord('<strong>THRUST</strong>', '<span id="voyager7_s3_thrust">0 N</span>')+
                _showRecord('<strong>BURN TIME</strong>', '<span id="voyager7_s3_burnTime">0 S</span>')+
            '</table>'+
        '</div>');
}

function showTitle() {
    startTag('div class="app-title"');
        print('<strong>VOYAGER 7 CONTROL PANEL</strong>');
    endTag('div');
}

function showTimer() {
    startTag('div id="current-time-info"');
        print(getActualDateTime().toUpperCase());
    endTag('div');
}

function showMissionTime() {
    startTag('div class="mission-time-info"');
        print('<strong>MISSION TIME: </strong><span id="voyager7_mission_time">00:00:00</span>');
    endTag('div');
}