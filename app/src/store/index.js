import Vue from 'vue'
import Vuex from 'vuex'
import axios from 'axios'
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
      const commit = { ...state.settings }

      Object.keys(payload).forEach(group => {
        if (commit[group]) {
          commit[group] = { ...commit[group], ...payload[group] }
        }
      })

      state.settings = commit
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
  },
  getters: {
    isSettingSupported: (state) => (key) => {
      const getDescendantProp = (obj, path) => {
        const arr = path.split(/[.[]['"]?/)
        let o = obj
        while (arr.length && o) {
          o = o[arr.shift().replace(/['"]?]$/, '')]
        }
        return o
      }

      return getDescendantProp(state.settings, key) !== undefined
    },
    getAvailableLanguages () {
      const list = []
      // const en = languages.en

      list.push({
        code: 'EN',
        name: 'English',
        native: 'English',
      })

      /*
      Object.keys(languages).forEach((code) => {
        list.push({
          code: code,
          name: languages[code].name,
          native: languages[code].native,
        })
      })
       */
      return list
    },

    getAvailableCountries () {
      return Object.values(timezones.getAllCountries())
    },

    getAvailableTimezones () {
      return Object.values(timezones.getAllTimezones())
    },
    getAvailableTimezone: () => (countryCode) => {
      return timezones.getCountry(countryCode)
    },
    getTimezoneByCountry: () => (country) => {
      return timezones.getTimezonesForCountry(country)
    },
    getTimezone: () => (timezone) => {
      return timezones.getTimezone(timezone)
    },
    getFSUsage: (state) => {
      return Math.round(
        (100 / state.stats.device.fs.total) * state.stats.device.fs.used,
      )
    },
    getMemoryUsage: (state) => {
      return Math.round(
        (100 / state.stats.device.heap.total) * state.stats.device.heap.free,
      )
    },

  },
})

export default store
