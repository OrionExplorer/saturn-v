var _external_tank = function() {
    this.id = -1;
    this.fuel = 0;
    this.attached = true;

    this.dryMass = 0;
    this.MAX_FUEL = 0;
    this.instrumentMass = 0;

    this.MAX_FUEL_BURN = 0;
    this.MAX_THRUST_N = 0;

    this.VARIABLE_THRUST = false;
    this.INITIAL_THRUST = 0;
    this.THRUST_STEP = 0;

    this.burn_start = false;
    this.burn_time = 0;

    this.center_engine_available = true;

    this.getBurnTime = function(currentEpoch) {
        if(this.burn_start > 0) {
            //return Math.round((currentEpoch - this.burn_start)/1000);
            return Math.round(this.burn_time);
        } else {
            return 0;
        }
    }

    this.getFuel = function() {
        return this.fuel;
    }
    
    this.getTotalMass = function() {
        return this.fuel + this.dryMass + this.instrumentMass;
    }

    this.getFuelPossibleAction = function() {
        return this.fuel < this.MAX_FUEL ? 'TANK' : 'RELEASE';
    }

    this.getAttached = function() {
        return this.attached == true ? 'ATTACHED' : 'STAGED';
    }

    this.getPossibleAction = function() {
        return this.attached == true ? 'ENGAGE' : 'ATTACH';
    }

    this.doAttach = function() {
        this.attached = true;
    }

    this.doDetach = function() {
        this.attached = false;
    }

    this.setFuel = function(val) {
        this.fuel = val;
    }
}

/*
 * FACTS:
 * S-IC dry =       130 421.868 KG
 * S-IC fuel =      2 145 798.09 KG (F 646 318.779 + O 1 499 479.31)
 * S-IC total =     2 278 688.4 KG      
 * 
 * S-II dry =       36 157.6622 KG
 * S-II fuel =      443 235.042 KG (F 71 720.2112 + O 371 514.831)
 * S-II total =         479 964 KG
 * 
 * S-IVB dry =      11 272.6776 KG
 * S-IVB fuel =     107 095.427 KG (F 87 315.1704 + O 19 780.2561)
 * S-IVB total =    119 119.253 KG
 * 
 * TOTAL VEHICLE =  2 938 314.67 KG
 */

var systemS1 = new _external_tank();
systemS1.id = 1;
systemS1.MAX_FUEL = 2106905 + 20311; //+ignition;
systemS1.dryMass = 132890;
systemS1.instrumentMass = 5206;//2278688-(systemS1.MAX_FUEL+systemS1.dryMass);
systemS1.MAX_FUEL_BURN = 13232; //(13232/10);
systemS1.VARIABLE_THRUST = true;
systemS1.INITIAL_THRUST =  34517594;
systemS1.MAX_THRUST_N = 39782855;
systemS1.THRUST_STEP = 33387;

var systemS2 = new _external_tank();
systemS2.id = 2;
systemS2.MAX_FUEL = 443235;
systemS2.dryMass = 36729;
systemS2.instrumentMass = 3663;//479964-(systemS2.MAX_FUEL+systemS2.dryMass);
systemS2.MAX_FUEL_BURN = 1224; //(1224/10);
systemS2.VARIABLE_THRUST = true;
systemS2.INITIAL_THRUST = 5095184;
systemS2.MAX_THRUST_N = 5095279;
systemS2.THRUST_STEP = 12;


var systemS3 = new _external_tank();
systemS3.id = 3;
systemS3.MAX_FUEL = 107095;
systemS3.dryMass = 12024;
systemS3.instrumentMass = 45693+4200;//119119-(systemS3.MAX_FUEL+systemS3.dryMass);
systemS3.MAX_THRUST_N = 901223;
systemS3.MAX_FUEL_BURN = 213; //(213/10);

var systemNULL = new _external_tank();
systemNULL.id = -1;
systemNULL.MAX_FUEL = 0;
systemNULL.dryMass = 0;
systemNULL.instrumentMass = 0;
systemNULL.MAX_THRUST_N = 0;
systemNULL.MAX_FUEL_BURN = 0;

var currentSystemStage = systemS1;