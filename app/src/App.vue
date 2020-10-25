<template>
  <v-app _style="background: #e2e2e2">
    <template v-if="isLoading">
      <v-overlay
        :absolute="true"
        :value="true"
      >
        <v-progress-circular
          indeterminate
          size="64"
        />
      </v-overlay>
    </template>

    <template v-else>
      <v-system-bar
        app
        dark
        color="primary"
        _window
        _lights-out
      >
        <span>paperdash.io</span>
        <v-spacer />
        <span>{{ settings.device.name }}</span>
        <v-spacer />
        <template v-if="stats.wifi.connected">
          <v-icon>{{ stats.wifi.rssi | wifiIcon(0) }}</v-icon>
          <span>{{ stats.device.time | moment("LT") }}</span>
        </template>
        <template v-else>
          <v-btn
            to="/setup/wifi"
            icon
          >
            <v-icon color="red">
              $signalWifiOff
            </v-icon>
          </v-btn>
        </template>
      </v-system-bar>

      <v-main>
        <v-container
          fluid
          class="align-start fill-height"
        >
          <transition-page>
            <router-view />
          </transition-page>
        </v-container>
      </v-main>
    </template>
  </v-app>
</template>

<script>
  import apiDevice from './api/device'
  import '@/assets/app.css'
  import transitionPage from '@/components/TransitionPage'

  export default {
    components: {
      transitionPage,
    },
    data: () => ({
      isLoading: true,
      settings: null,
    }),
    computed: {
      stats () {
        return this.$root._data.stats
      },
    },
    watch: {
      stats () {
        this.isLoading = false
      },
    },
    created () {
      apiDevice.getSettings(settings => {
        this.settings = settings

        this.autoReloadStats()
      })
    },
    methods: {
      autoReloadStats () {
        apiDevice.getStats(stats => {
          // give esp some extra time befor fetch new data
          stats.playlist.remaining += 2

          // reset old so reactive watcher can detect a change
          if (this.$root._data.stats) {
            this.$root._data.stats.playlist.remaining = 0
          }
          this.$root._data.stats = stats

          setTimeout(() => {
            this.autoReloadStats()
          }, stats.playlist.remaining * 1000)
        })
      },
    },
  }
</script>

<style scoped>
</style>
