var gravitationalConstant = 6.67*Math.pow(10, -11);
var AU = 149598000000;

var _astronomicalObject = function() {
    this.name = undefined;
    this.gravitation = undefined;
    this.mass = undefined;
    this.orbitRadius = undefined;

    this.groundDestination = undefined;
    this.groundDestinationAltitude = undefined;

    this.spaceDestination = undefined;

    this.currentPosition = undefined; //0 - ground, 1 - orbit

    this.getCurrentDestination = function() {
        return this.currentPosition == 0 ? this.groundDestination : this.spaceDestination;
    }

    this.getCurrentDestinationAltitude = function() {
        return this.currentPosition == 0 ? this.groundDestinationAltitude : null;
    }

    this.getOrbitalSpeed = function(height) {
        return Math.sqrt((gravitationalConstant*(this.mass))/(this.radius+height));
    }

    this.getAltitudeFromVelocity = function(velocity) {
        //credit to Andrzej Grodtke
        return (gravitationalConstant*(this.mass/Math.pow(velocity, 2))) - this.radius;
    }

    this.getGravityValue = function(height) {
        if(height >= 0) {
            return this.gravitation*Math.pow((this.radius/(this.radius+height)), 2); //6367445
        } else {
            return this.gravitation;
        }
    }
}

var AO_Earth = new _astronomicalObject();
AO_Earth.name = 'EARTH';
AO_Earth.gravitation = 9.82;
AO_Earth.mass = 5.98*Math.pow(10,24);
AO_Earth.radius = 6378159;//6370000;
AO_Earth.orbitRadius = 924375700;
AO_Earth.groundDestination = 'EPO';
AO_Earth.groundDestinationAltitude = 190756;
AO_Earth.currentPosition = 0;

var AO_Moon = new _astronomicalObject();
AO_Moon.name = 'MOON';
AO_Moon.gravitation = 1.63;
AO_Moon.mass = 7.34*Math.pow(10,22);
AO_Moon.radius = 1737000;//173114;
AO_Moon.orbitRadius = 2413402000;//384399000;//*2*Math.PI;// ;2.41902634*Math.pow(10,9);
AO_Moon.groundDestination = 'LO';
AO_Moon.groundDestinationAltitude = 111120;
AO_Moon.currentPosition = 0;

var currentAO = AO_Earth;