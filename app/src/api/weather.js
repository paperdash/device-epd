/**
 * Mocking client-server processing
 */

// eslint-disable-next-line
const _weather = {
	coord: { lon: 13.04, lat: 47.8 },
	weather: [
		{ id: 800, main: "Clear", description: "Klarer Himmel", icon: "01d" }
	],
	base: "stations",
	main: {
		temp: 2.75,
		feels_like: 0.09,
		temp_min: 0,
		temp_max: 6.11,
		pressure: 1023,
		humidity: 69
	},
	visibility: 10000,
	wind: { speed: 0.5 },
	clouds: { all: 9 },
	dt: 1584260150,
	sys: {
		type: 1,
		id: 6877,
		country: "AT",
		sunrise: 1584249628,
		sunset: 1584292379
	},
	timezone: 3600,
	id: 2766824,
	name: "Salzburg",
	cod: 200
}

// eslint-disable-next-line
const _forecast = {
	city: {
		id: 2766824,
		name: "Salzburg",
		coord: { lon: 13.044, lat: 47.7994 },
		country: "AT",
		population: 0,
		timezone: 3600
	},
	cod: "200",
	message: 0.0878612,
	cnt: 4,
	list: [
		{
			dt: 1584270000,
			sunrise: 1584249627,
			sunset: 1584292378,
			temp: {
				day: 14.02,
				min: 1.59,
				max: 14.02,
				night: 1.59,
				eve: 6.4,
				morn: 2.88
			},
			feels_like: { day: 9.75, night: -2.31, eve: 2.85, morn: -0.11 },
			pressure: 1021,
			humidity: 42,
			weather: [
				{
					id: 801,
					main: "Clouds",
					description: "Ein paar Wolken",
					icon: "02d"
				}
			],
			speed: 3.55,
			deg: 105,
			clouds: 15
		},
		{
			dt: 1584356400,
			sunrise: 1584335904,
			sunset: 1584378865,
			temp: {
				day: 13.15,
				min: 0.88,
				max: 13.15,
				night: 4.3,
				eve: 6.25,
				morn: 0.88
			},
			feels_like: { day: 10.56, night: 1.77, eve: 4.16, morn: -2.11 },
			pressure: 1022,
			humidity: 46,
			weather: [
				{
					id: 800,
					main: "Clear",
					description: "Klarer Himmel",
					icon: "01d"
				}
			],
			speed: 1.26,
			deg: 349,
			clouds: 0
		},
		{
			dt: 1584442800,
			sunrise: 1584422182,
			sunset: 1584465351,
			temp: {
				day: 13.8,
				min: 3.73,
				max: 14.05,
				night: 9.64,
				eve: 9.75,
				morn: 3.73
			},
			feels_like: { day: 11.71, night: 7.26, eve: 7.47, morn: 1.33 },
			pressure: 1029,
			humidity: 53,
			weather: [
				{ id: 804, main: "Clouds", description: "Bedeckt", icon: "04d" }
			],
			speed: 1.21,
			deg: 331,
			clouds: 100
		},
		{
			dt: 1584529200,
			sunrise: 1584508459,
			sunset: 1584551838,
			temp: {
				day: 16.55,
				min: 6.35,
				max: 16.55,
				night: 7.1,
				eve: 9.94,
				morn: 6.35
			},
			feels_like: { day: 14.61, night: 5.37, eve: 8.03, morn: 4.56 },
			pressure: 1028,
			humidity: 52,
			weather: [
				{
					id: 800,
					main: "Clear",
					description: "Klarer Himmel",
					icon: "01d"
				}
			],
			speed: 1.67,
			deg: 39,
			clouds: 7
		}
	]
}

import axios from 'axios'

export default {

    /**
     * @param cb
     * @returns {PromiseLike<any> | Promise<any>}
     */
	getCurrent(cb) {
		//return cb(_weather);

		// eslint-disable-next-line
		return axios
			.get('/fs/weatherCurrent.json')
			.then(response => cb(response.data))
	},

    /**
     * @param cb
     * @returns {PromiseLike<any> | Promise<any>}
     */
	getForecast(cb) {
		//return cb(_forecast);

		// eslint-disable-next-line
		return axios
			.get('/fs/weatherForecast.json')
			.then(response => cb(response.data))
	},

	/**
     * @param cb
     * @returns {PromiseLike<any> | Promise<any>}
     */
	updateWeather(cb) {
		return axios
			.get('/api/update?weather=1')
			.then(response => cb(response.data))
	},
}
