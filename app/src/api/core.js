/**
 * Mocking client-server processing
 */

const _settings = {
	"wifi_ssid": "",

	"device_mode": "active",

	// deep sleep
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
