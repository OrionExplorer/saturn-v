var _antenna = function() {
    this.engaged = false;
    this.name = undefined;

    this.doEngage = function() {
        this.engaged = true;
    }

    this.doDisengage = function() {
        this.engaged = false;
    }

    this.getPossibleAction = function() {
        return this.engaged == true? 'DEACTIVATE' : 'ACTIVATE';
    }

    this.getEngagedStatus = function() {
        return this.engaged == true ? 'ACTIVE' : 'INACTIVE';
    }
}

var highGainAntenna = new _antenna();
var lowGainAntenna = new _antenna();