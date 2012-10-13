function getActualDateTime() {
    var currentDate = new Date(),
    result = currentDate;
    delete currentDate;
    return result.toGMTString();
}

function updateTime() {
    var timeStr = document.getElementById('current-time-info');
    timeStr.innerHTML = getActualDateTime().toUpperCase();
    setTimeout('updateTime()', 1000);
}

function secondsToHms(d) {
    var lessThanZero = (d < 0) ? true : false;
    d = Math.abs(Math.round(Number(d)));

    var h = Math.floor(d / 3600);
    var m = Math.floor(d % 3600 / 60);
    var s = Math.floor(d % 3600 % 60);
    return lessThanZero ? 'T-'+(h < 10 ? '0'+h : h)+':'+(m < 10 ? '0'+m : m)+':'+(s < 10 ? '0'+s : s) : 'T+'+(h < 10 ? '0'+h : h)+':'+(m < 10 ? '0'+m : m)+':'+(s < 10 ? '0'+s : s);
}