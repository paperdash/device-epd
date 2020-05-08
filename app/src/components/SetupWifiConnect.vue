<template>
  <v-card flat>
    <v-toolbar dark color="primary">
      <v-btn icon dark @click="dialog = false">
        <v-icon>$close</v-icon>
      </v-btn>
      <v-toolbar-title class="pl-0">Enter the password for "{{ ssid }}"</v-toolbar-title>

      <v-progress-linear
        :active="isConnecting"
        indeterminate
        absolute
        bottom
        color="deep-orange accent-4"
      ></v-progress-linear>
    </v-toolbar>

    <v-card-text class="pa-5">
      <v-text-field
        v-model="password"
        :append-icon="show1 ? 'mdi-eye' : 'mdi-eye-off'"
        :type="show1 ? 'text' : 'password'"
        label="i8n:Password"
        @click:append="show1 = !show1"
      ></v-text-field>
    </v-card-text>

    <v-card-actions>
      <v-spacer></v-spacer>
      <v-btn depressed :disabled="isConnecting" color="primary" @click="onWifiConnect()">i8n:Join</v-btn>
    </v-card-actions>
  </v-card>
</template>

<script>
import apiDevice from "@/api/device";

export default {
  props: {
    ssid: {
      type: String,
      required: true
    }
  },
  data: () => ({
    isConnecting: false,
    //wifiAvailable: [],
    //wifiConnectModal: false,
    //wifiConnectSSID: null,
    password: null,
    show1: false
  }),
  methods: {
    onWifiConnect() {
      this.isConnecting = true;

      apiDevice.wifiConnect(this.ssid, this.password);
    }
  }
};
</script>
