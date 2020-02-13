/**
 * Mocking client-server processing
 */

const _settings = {
	// connected wifi
	"wifi_ssid": "",

	// oparation mode
	"device_mode": "active",

	// Set rotation setting for display
	// 0 thru 3 corresponding to 4 cardinal rotations
	"device_rotation": 0,

	// deep sleep timer
	"cloud_refresh": 97
}

import axios from 'axios'

export default {

    /**
     * @param cb
     * @returns {PromiseLike<any> | Promise<any>}
     */
	getSettings(cb) {

		return _settings

		return axios
			.get('/api/settings')
			.then(response => cb(response.data))
	}
}
