<template>
  <v-container fluid _fill-height>
    <v-card flat class="mx-auto" width="520">
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
        <v-list-item-group v-model="settings.wifi" mandatory>
          <template v-for="(wifi, i) in wifiAvailable">
            <div :key="i">
              <v-divider v-if="i > 0"></v-divider>

              <v-list-item class="px-1" :value="wifi.ssid" @click.stop="wifiConnectModal = true">
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
        </v-list-item-group>

        <v-divider></v-divider>

        <v-btn text color="primary" class="px-0 my-2">Choose Another Network</v-btn>
      </v-list>

      <v-dialog v-model="wifiConnectModal" max-width="450">
        <setup-wifi-connect
          :ssid="settings.wifi"
          @connected="commitWifi()"
          @cancel="wifiConnectModal = false"
        ></setup-wifi-connect>
      </v-dialog>

    </v-card>
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

    wifiAvailable: [],
    wifiConnectModal: false
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
  methods: {}
};
</script>