/**
 * Mocking client-server processing
 */

// eslint-disable-next-line
const _settings = {
	// connected wifi
	"wifi_ssid": "",

	// oparation mode
	"device_mode": "active",

	// Set rotation setting for display
	// 0 thru 3 corresponding to 4 cardinal rotations
	"device_rotation": 0,

	// clound server endpoint
	"cloud_server": ""
}

// eslint-disable-next-line
const _wifiScan = [{ "rssi": -59, "ssid": "xd-design.info", "bssid": "38:10:D5:34:80:1B", "channel": 11, "secure": 3 }, { "rssi": -75, "ssid": "FRITZ!Box 7430 JI", "bssid": "38:10:D5:5D:FE:7C", "channel": 1, "secure": 3 }, { "rssi": -87, "ssid": "Vodafone Hotspot", "bssid": "AA:0E:14:BD:50:ED", "channel": 1, "secure": 0 }, { "rssi": -88, "ssid": "WLAN-548426", "bssid": "E0:60:66:55:7F:C5", "channel": 1, "secure": 3 }, { "rssi": -89, "ssid": "Familie Kalinowski", "bssid": "C8:0E:14:BD:50:ED", "channel": 1, "secure": 3 }, { "rssi": -91, "ssid": "WLAN-507287", "bssid": "E0:60:66:48:6C:6B", "channel": 1, "secure": 3 }, { "rssi": -94, "ssid": "TP-LINK_7238", "bssid": "A4:2B:B0:D8:72:38", "channel": 3, "secure": 3 }]

import axios from 'axios'

export default {

    /**
     * @param cb
     * @returns {PromiseLike<any> | Promise<any>}
     */
	getSettings(cb) {
		return axios
			.get('/api/settings')
			.then(response => cb(response.data))
	},


	/**
	 * @returns {IDBRequest<IDBValidKey> | Promise<void>}
	 */
	putSettings(settings, cb) {
		return axios
			.put('/api/settings', settings, {
				headers: {
					'Content-Type': 'application/json'
				}
			})
			.then(response => cb(response.data))
	},


	/**
	 * scan for wifi in range
	 * @param {*} cb
	 */
	wifiScan(cb) {
		return cb(_wifiScan)

		// eslint-disable-next-line
		return axios
			.get('/api/wifi/scan')
			.then(response => cb(response.data))
	},



	wifiConnect(ssid, password, cb) {
		return axios
			.post('/api/wifi/connect', {
				ssid: ssid,
				password: password
			}, {
				headers: {
					'Content-Type': 'application/json'
				}
			})
			.then(response => cb(response.data))
	}
}
