function updateEl(id, val, callback) {
    id = document.getElementById(id);
    switch(id.type) {
        case 'button' : {
                id.value = val;
        }break;
        case undefined : {
                id.innerHTML = val;
        }break;
        default : {
                id.innerHTML = val;
        }break;
    }
    
    if(callback != undefined) {
        eval(callback);
    }
}

function updateHUD(id, val, delay, callback) {
    if(delay != undefined) {
        if(callback != undefined) {
            setTimeout('updateEl(\''+id+'\', \''+val+'\', \''+callback+'\')', delay);
        } else {
            setTimeout('updateEl(\''+id+'\', \''+val+'\')', delay);
        }
    } else {
        updateEl(id, val);
    }
}

function disableEl(id) {
    document.getElementById(id).disabled = true;
}

function enableEl(id) {
    document.getElementById(id).disabled = false;
}

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

function setAllButtonsDisabled(bool) {
    var inputs = document.getElementsByTagName("input");
    for (var i = 0; i < inputs.length; i++) {
        if (inputs[i].type === 'button') {
            inputs[i].disabled = bool;
        }
    }
}

function updateInformation(status) {
    var infoPanel = document.getElementById('status_text');
    var _status = status.toString().toUpperCase();
    infoPanel.innerHTML = '<strong>STATUS: '+_status+'.</strong>';
}

function json2text(json_object) {
    function replacer(key, value) {
        if (typeof value === 'number' && !isFinite(value)) {
            return String(value);
        }
        return value;
    }
    
    return JSON.stringify(json_object, replacer);
}

setAllButtonsDisabled(true);
setTimeout("updateTime()", 1000);

if( checkBrowserForHTML5() ) {
    getVoyager7RemoteAddress();
} else {
    updateInformation('Error: Please check your browser for HTML5 support');
}