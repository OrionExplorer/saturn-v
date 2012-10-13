function setAllButtonsDisabled(bool) {
    var inputs = document.getElementsByTagName("input");
    for (var i = 0; i < inputs.length; i++) {
        if (inputs[i].type === 'button') {
            inputs[i].disabled = bool;
        }
    }
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
function startTag(tag) {
    document.write('<'+tag+'>');
}

function endTag(tag) {
    document.write('</'+tag+'>');
}

function print(str) {
    document.write(str);
}

function _showRecord(property, value) {
    return '<tr><td class="background-property-cell">'+property+'</td><td class="background-value-cell">'+value+'</td></tr>';
}

function showButton(caption, callback, id) {
    return '<input type="button" id="'+id+'" value="'+caption+'" onClick="'+callback+'(\''+id+'\');">';
}

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