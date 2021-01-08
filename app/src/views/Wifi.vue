<template>
  <v-card
    flat
    rounded="lg"
  >
    <v-card-title class="display-2 mb-12 justify-center text-center">
      Wifi settings
    </v-card-title>

    <v-list>
      <template v-if="stats.wifi.connected">
        <v-list-item
          class="px-1"
        >
          <v-list-item-icon class="mr-2 ml-2">
            <v-icon>
              $check
            </v-icon>
          </v-list-item-icon>

          <v-list-item-content dark>
            <v-list-item-title v-text="stats.wifi.ssid" />
            <v-list-item-subtitle v-text="stats.wifi.mac" />
          </v-list-item-content>

          <v-list-item-icon>
            <v-icon
              v-if="stats.wifi.secure"
              class="mx-2"
            >
              $lock
            </v-icon>
            <v-icon class="mx-2">
              {{ stats.wifi.rssi | wifiIcon(0) }}
            </v-icon>
            <v-btn
              icon
              disabled
            >
              <v-icon>
                $info
              </v-icon>
            </v-btn>
          </v-list-item-icon>
        </v-list-item>
        <v-divider />
      </template>
      <v-subheader class="pl-0 mt-5">
        Networks
      </v-subheader>

      <template v-if="!isLoading">
        <template

          v-for="(wifi, i) in wifiAvailable"
        >
          <div
            v-if="wifi.ssid !== settings.system.wifi || !stats.wifi.connected"
            :key="i"
          >
            <v-divider v-if="i > 0" />

            <v-list-item
              class="px-1"
              @click="onWifiSelect(wifi)"
            >
              <v-list-item-icon class="mr-2 ml-2">
                <v-icon v-if="wifi.ssid === settings.system.wifi">
                  $check
                </v-icon>

                <v-progress-circular
                  v-if="wifi.ssid === connectingSSID"
                  :size="24"
                  :width="2"
                  color="grey "
                  indeterminate
                />
              </v-list-item-icon>

              <v-list-item-content dark>
                <v-list-item-title v-text="wifi.ssid" />
                <v-list-item-subtitle v-text="wifi.bssid" />
              </v-list-item-content>

              <v-list-item-icon>
                <v-icon
                  v-if="wifi.secure"
                  class="mx-2"
                >
                  $lock
                </v-icon>
                <v-icon class="mx-2">
                  {{ wifi.rssi | wifiIcon(0) }}
                </v-icon>
                <v-icon class="ml-3">
                  $next
                </v-icon>
              </v-list-item-icon>
            </v-list-item>
          </div>
        </template>
      </template>

      <v-skeleton-loader
        v-else
        type="list-item-two-line,list-item-two-line,list-item-two-line"
      />

      <v-divider />

      <v-btn
        text
        color="primary"
        class="_px-0 my-2"
        disabled
      >
        Choose Another Network
      </v-btn>
    </v-list>

    <v-dialog
      v-model="wifiPasswordModal"
      max-width="450"
      _:fullscreen="$vuetify.breakpoint.mobile"
    >
      <setup-wifi-connect
        :ssid="connectSSID"
        :on-connect="onWifiConnect"
        @cancel="wifiPasswordModal = false"
      />
    </v-dialog>
  </v-card>
</template>

<script>
  import { mapState } from 'vuex'
  import setupWifiConnect from '@/components/SetupWifiConnect'
  import apiDevice from '@/api/device'

  export default {
    components: {
      setupWifiConnect,
    },
    data: () => ({
      isLoading: true,

      connectSSID: null,
      connectingSSID: '',

      wifiAvailable: [],
      wifiPasswordModal: false,
    }),
    computed: {
      ...mapState(['stats', 'settings']),
    },
    created () {
      apiDevice.wifiScan(list => {
        this.wifiAvailable = list

        this.isLoading = false
      })
    },
    methods: {
      onWifiSelect (wifi) {
        if (wifi.secure) {
          this.connectSSID = wifi.ssid
          this.wifiPasswordModal = true
        } else {
          this.onWifiConnect(wifi.ssid, '')
        }
      },
      onWifiConnect (ssid, password) {
        this.connectingSSID = ssid
        this.wifiPasswordModal = false

        apiDevice.wifiConnect(ssid, password, () => {
          setTimeout(() => {
            // phone should be back in regular wifi
            // search for device via mDNS
            window.location = 'http://paperdash-epd.local'
          }, 5000)
        })
      },
    },
  }
</script>

<style scoped>
</style>
