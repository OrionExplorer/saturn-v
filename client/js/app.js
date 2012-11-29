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

function disableEl(id) {
    document.getElementById(id).disabled = true;
}

function enableEl(id) {
    document.getElementById(id).disabled = false;
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
    var infoPanel = document.getElementById('message-box');
    var _status = status.toString()/*.toUpperCase()*/;
    infoPanel.value += '\n'+_status;
    infoPanel.scrollTop = 99999;
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

function showApolloLMControlPanel() {
    var apolloCSM_control = document.getElementById('apolloCSM_control');
    var apolloLM_control = document.getElementById('apolloLM_control');
    var sa514_control = document.getElementById('sa514_control');
    apolloCSM_control.style.display = 'none';
    sa514_control.style.display = 'none';
    apolloLM_control.style.display = 'block';
}

function showApolloCSMControlPanel() {
    var apolloCSM_control = document.getElementById('apolloCSM_control');
    var apolloLM_control = document.getElementById('apolloLM_control');
    var sa514_control = document.getElementById('sa514_control');
    apolloLM_control.style.display = 'none';
    sa514_control.style.display = 'none';
    apolloCSM_control.style.display = 'block';
}

function showApolloSA514ControlPanel() {
    var apolloCSM_control = document.getElementById('apolloCSM_control');
    var apolloLM_control = document.getElementById('apolloLM_control');
    var sa514_control = document.getElementById('sa514_control');
    apolloLM_control.style.display = 'none';
    apolloCSM_control.style.display = 'none';
    sa514_control.style.display = 'block';
}

setAllButtonsDisabled(true);

if( checkBrowserForHTML5() ) {
    getVoyager7RemoteAddress();
} else {
    updateInformation('Error: Please check your browser for HTML5 support');
}