const fz = require('zigbee-herdsman-converters/converters/fromZigbee');
//const tz = require('zigbee-herdsman-converters/converters/toZigbee');
const exposes = require('zigbee-herdsman-converters/lib/exposes');
const reporting = require('zigbee-herdsman-converters/lib/reporting');
//const extend = require('zigbee-herdsman-converters/lib/extend');
const ota = require('zigbee-herdsman-converters/lib/ota');
const e = exposes.presets;
//const ea = exposes.access;


const definition = {
    zigbeeModel: ['ESP32H2 Router Dev'], // The model ID from: Device with modelID 'lumi.sens' is not supported.
    model: 'WSDCGQ01LM', // Vendor model number, look on the device for a model number
    vendor: 'esp32-*', // Vendor of the device (only used for documentation and startup logging)
    description: 'temeperature and humidity', // Description of the device, copy from vendor site. (only used for documentation and startup logging)
    fromZigbee: [fz.temperature, fz.humidity, fz.battery], // We will add this later
    toZigbee: [], // Should be empty, unless device can be controlled (e.g. lights, switches).
    configure: async (device, coordinatorEndpoint, logger) => {
        const endpoint = device.getEndpoint(10);
        const bindClusters = ['msTemperatureMeasurement', 'msRelativeHumidity', 'genPowerCfg'];
        await reporting.bind(endpoint, coordinatorEndpoint, bindClusters);
        await reporting.temperature(endpoint, {min: 300, max: 3600, change: 0.5});
        await reporting.humidity(endpoint, {min: 300, max: 3600, change: 1});
//        await reporting.batteryVoltage(endpoint, {min: 30, max: 3600, change: 1});
        await reporting.batteryPercentageRemaining(endpoint, {min: 300, max: 3600, change: 1});
    },
    exposes: [e.temperature(), e.humidity(), e.battery()], // Defines what this device exposes, used for e.g. Home Assistant discovery and in the frontend
    ota: ota.zigbeeOTA,
};

module.exports = definition;
