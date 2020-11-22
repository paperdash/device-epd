import Vue from 'vue'
import Vuex from 'vuex'
import axios from 'axios'
// import device from '@/api/device'

import { countries, languages } from 'countries-list'
import timezones from 'countries-and-timezones'

Vue.use(Vuex)

const store = new Vuex.Store({
  state: {
    stats: {},
    settings: {},
    notifications: {},
  },
  mutations: {
    setStats (state, payload) {
      state.stats = payload
    },
    setSettings (state, payload) {
      state.settings = payload
    },
    notification (state, payload) {
      state.notifications = payload
    },
    updateSettings (state, payload) {
      state.settings = { ...state.settings, ...payload }
      console.log(state.settings)
    },
  },
  actions: {
    async loadStats ({ commit }) {
      try {
        const response = await axios.get('/stats')
        commit('setStats', response.data)
      } catch (error) {
        commit('setStats', {})
      }
    },
    async loadSettings ({ commit }) {
      try {
        const response = await axios.get('/api/settings')
        commit('setSettings', response.data)
      } catch (error) {
        commit('setSettings', {})
      }
    },
    async saveSettings ({ commit, state }) {
      try {
        await axios.put('/api/settings', state.settings, {
          headers: {
            'Content-Type': 'application/json',
          },
        })
        commit('notification', 'settings saved')
      } catch (error) {
        console.warn(error)
      }
    },

    /*
    async getSensors ({ commit }) {
      try {
        const response = await axios.get('/api/sensors')
        commit('setSensors', response.data)
      } catch (error) {
        commit('setSensors', [])
      }
    },
    async putSensor ({ commit }, [id, sensor]) {
      try {
        await axios.put('/api/sensor/' + id, {
          label: sensor.label,
        })
        commit('updateSensor', sensor)
      } catch (error) {
        console.warn(error)
      }
    },
    async deleteSensor ({ commit }, id) {
      try {
        await axios.delete('/api/sensor/' + id)
        commit('deleteSensor', id)
        commit('notification', 'sensor #' + id + ' deleted')
      } catch (error) {
        console.warn(error)
      }
    },

     */
  },
  getters: {
    getAvailableLanguages () {
      return languages
    },

    getAvailableCountries () {
      return countries
    },

    getAvailableTimezones () {
      return timezones.getAllTimezones()
    },
    getAvailableTimezone: () => (countryCode) => {
      return timezones.getCountry(countryCode)
    },
  },
})

// sensor push data
/*
const connection = new WebSocket('ws://' + window.location.host + '/ws')
connection.onmessage = (message) => {
  const log = JSON.parse(message.data)
  log.last_update = new Date()

  store.commit('updateSensor', log)
  store.commit('addSensorHistory', log)
  store.commit('notification', log)
}

store.watch(
  state => state.pushUpdate,
  (value) => {
    if (value) {
      console.info('TODO:: enable websocket')
    } else {
      console.info('TODO:: disable websocket')
    }
  },
)
*/
export default store
