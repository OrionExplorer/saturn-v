function showInformation(status) {
    print('<div class="tooltip"><span id="status_text"></span></div>');
    showMissionTime();
    updateInformation(status);
}
function updateInformation(status) {
    var infoPanel = document.getElementById('status_text');
    var _status = status.toString().toUpperCase();
    infoPanel.innerHTML = '<strong>STATUS: '+_status+'.</strong>';
}