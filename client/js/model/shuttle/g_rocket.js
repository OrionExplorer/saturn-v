/**
 * currentSpeed
 * Actual spacecraft speed in kilometers per second
 **/
var currentSpeed = 0.0;

/**
 * MAX_THRUST
 * Maximum allowed thrust power.
 */
var MAX_THRUST = 100.0;

/**
 * SPACECRAFT_MASS
 * Voyager 7 mass (without ETC)
 */
var SPACECRAFT_MASS = 60543;

/**
 * @private
 * _thruster
 * A simple engine declaration:
 * @engaged {Boolean} - information about engine status (on/off).
 * @thrust {Float} - information about current thrust power.
 * @name {String} - thruster name (ie. MAIN ENGINE)
 */
var _thruster = function() {
    this.engaged = false;
    this.thrust = 0.0;
    this.name = undefined;
    
    this.doEngage = function() {
        this.engaged = true;
    }

    this.doDisengage = function() {
        this.engaged = false;
    }

    this.setThrust = function(val) {
        this.thrust = val;
    }

    this.getThrust = function() {
        return this.thrust;
    };

    this.getPossibleAction = function() {
        return this.engaged == true? 'DISENGAGE' : 'ENGAGE';
    }

    this.getEngagedStatus = function() {
        return this.engaged == true ? 'ON' : 'OFF';
    }
}

/**
 * thrusters
 * Voyager 7 consists of 4 thrusters (see @_thruster).
 */
var thrusters = [ new _thruster, new _thruster, new _thruster, new _thruster];
thrusters[0].name = 'FORWARD THRUST';
thrusters[1].name = 'SIDE THRUST';
thrusters[2].name = 'UP THRUST';
thrusters[3].name = 'DOWN THRUST';

/**
 * mainEngine
 * Voyager 7 Main Engine (see @_thruster).
 */
var mainEngine = new _thruster();

/**
 * systemAPS
 * Ascent Propulsion System (see @_thruster).
 */
var systemAPS = new _thruster();

/**
 * systemDPS
 * Descent Propulsion System (see @_thruster).
 */
var systemDPS = new _thruster();

var launchEscapeTowerReady = true;