<template>
  <setup-panel
    back
    @back="stepBack"
  >
    <template #icon>
      $wifi
    </template>
    <template #headline>
      Choose a
      <br>Wi-Fi Network
    </template>

    <v-skeleton-loader
      v-if="isLoading"
      type="list-item-two-line,list-item-two-line,list-item-two-line"
      class="mx-auto"
    />

    <v-list v-else>
      <template v-for="(wifi, i) in wifiAvailable">
        <div :key="i">
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
    >
      <setup-wifi-connect
        :ssid="connectSSID"
        :on-connect="onWifiConnect"
        @cancel="wifiPasswordModal = false"
      />
    </v-dialog>
  </setup-panel>
</template>

<script>
  import apiDevice from '@/api/device'
  import setupWifiConnect from '@/components/Setup/WifiConnect'
  import SetupPanel from '@/components/Setup/BasePanel'

  export default {
    components: {
      SetupPanel,
      setupWifiConnect,
    },
    data: () => ({
      isLoading: true,
      settings: null,

      connectSSID: null,
      connectingSSID: '',

      wifiAvailable: [],
      wifiPasswordModal: false,
    }),
    created () {
      apiDevice.getSettings(settings => {
        this.settings = settings

        apiDevice.wifiScan(list => {
          this.wifiAvailable = list

          this.isLoading = false
        })
      })
    },
    methods: {
      onWifiSelect (wifi) {
        if (wifi.ssid === this.settings.system.wifi) {
          this.nextStep()
        } else if (wifi.secure) {
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
      stepBack () {
        this.$router.push('/setup/country')
      },
      nextStep () {
        this.$router.push('/setup/weather')
      },
    },
  }
</script>
