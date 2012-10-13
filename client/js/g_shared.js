var totalDistance = 0.0; /* m */
var currentAltitude = 0.0;
var lastVelocity = 0;

var timeInterval = 100;
var missionTime = -7;
var ascendingTime = -1;

function showInformation(status) {
    print('<div class="tooltip"><span id="status_text"></span></div>');
    showMissionTime();
    updateInformation(status);
}
function updateInformation(status) {
    var infoPanel = document.getElementById('status_text');
    var _status = status.toString().toUpperCase();
    infoPanel.innerHTML = '<strong>CURRENT STATUS: '+_status+'.</strong>';
    if(_status != 'IDLE') {
        console.log('CURRENT STATUS: '+_status);
    }
    if(status != 'IDLE') {
        setTimeout(function() {
            updateInformation('IDLE');
        }, 6000);
    }
}