<template>
  <v-container fluid _fill-height>
    <v-row no-gutters justify="center">
      <v-col lg="5" md="6" sm="8">
        <v-card flat>
          <v-card-title class="display-2 mb-12 justify-center text-center">
            Choose a
            <br />Wi-Fi Network
          </v-card-title>

          <v-skeleton-loader
            v-if="isLoading"
            type="list-item-two-line,list-item-two-line,list-item-two-line"
            class="mx-auto"
          ></v-skeleton-loader>

          <v-list v-else>
            <template v-for="(wifi, i) in wifiAvailable">
              <div :key="i">
                <v-divider v-if="i > 0"></v-divider>

                <v-list-item class="px-1" @click="onWifiSelect(wifi)">
                  <v-list-item-icon class="mr-2 ml-2">
                    <v-icon v-if="wifi.ssid === settings.system.wifi">$check</v-icon>

                    <v-progress-circular
                      v-if="wifi.ssid === connectingSSID"
                      :size="24"
                      :width="2"
                      color="grey "
                      indeterminate
                    ></v-progress-circular>
                  </v-list-item-icon>

                  <v-list-item-content dark>
                    <v-list-item-title v-text="wifi.ssid"></v-list-item-title>
                    <v-list-item-subtitle v-text="wifi.bssid"></v-list-item-subtitle>
                  </v-list-item-content>

                  <v-list-item-icon>
                    <v-icon class="mx-2" v-if="wifi.secure">$lock</v-icon>
                    <v-icon class="mx-2">{{ wifi.rssi | wifiIcon(0) }}</v-icon>
                    <v-icon class="ml-3">$next</v-icon>
                  </v-list-item-icon>
                </v-list-item>
              </div>
            </template>

            <v-divider></v-divider>

            <v-btn text color="primary" class="_px-0 my-2">Choose Another Network</v-btn>
          </v-list>

          <v-dialog v-model="wifiPasswordModal" max-width="450">
            <setup-wifi-connect
              :ssid="connectSSID"
              :onConnect="onWifiConnect"
              @cancel="wifiPasswordModal = false"
            ></setup-wifi-connect>
          </v-dialog>
        </v-card>
      </v-col>
    </v-row>
  </v-container>
</template>

<script>
import apiDevice from "@/api/device";
import setupWifiConnect from "@/components/SetupWifiConnect";

export default {
  components: {
    setupWifiConnect
  },
  data: () => ({
    isLoading: true,
    settings: null,

    connectSSID: null,
    connectingSSID: "",

    wifiAvailable: [],
    wifiPasswordModal: false
  }),
  created() {
    apiDevice.getSettings(settings => {
      this.settings = settings;

      apiDevice.wifiScan(list => {
        this.wifiAvailable = list;

        this.isLoading = false;
      });
    });
  },
  methods: {
    onWifiSelect(wifi) {
      if (wifi.secure) {
        this.connectSSID = wifi.ssid;
        this.wifiPasswordModal = true;
      } else {
        this.onWifiConnect(wifi.ssid, "");
      }
    },
    onWifiConnect(ssid, password) {
      this.connectingSSID = ssid;
      this.wifiPasswordModal = false;

      apiDevice.wifiConnect(ssid, password, () => {});
    }
  }
};
</script>