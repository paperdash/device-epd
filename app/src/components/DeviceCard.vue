<template>
  <v-card outlined>
    <v-list-item>
      <v-list-item-icon class="mr-3">
        <v-icon>{{ stats.wifi.rssi | wifiIcon(stats.wifi.secure) }}</v-icon>
      </v-list-item-icon>
      <v-list-item-content>
        <v-list-item-title>{{ stats.wifi.ssid }}</v-list-item-title>
      </v-list-item-content>
      <v-list-item-avatar>
        <v-avatar color="secondary lighten-3" size="24">
          <span class="white--text headline caption">{{ stats.wifi.channel }}</span>
        </v-avatar>
      </v-list-item-avatar>
    </v-list-item>

    <v-divider class="mx-4"></v-divider>

    <v-list dense>
      <v-list-item v-for="key in wifiStats" :key="key">
        <v-list-item-title>{{ key }}</v-list-item-title>

        <v-list-item-subtitle class="text-right">{{ stats.wifi[key] }}</v-list-item-subtitle>
      </v-list-item>
    </v-list>

    <v-list-item>
      <v-list-item-icon class="mr-3">
        <v-icon>$memory</v-icon>
      </v-list-item-icon>
      <v-list-item-content>
        <v-list-item-title>i8n:Storage</v-list-item-title>
      </v-list-item-content>
      <v-list-item-avatar>
        <v-progress-circular :rotate="-90" :value="fsUsage" class="caption">{{ fsUsage }}</v-progress-circular>
      </v-list-item-avatar>
    </v-list-item>

    <v-divider class="mx-4"></v-divider>

    <v-list dense class="pb-0">
      <v-list-item>
        <v-list-item-title>Total</v-list-item-title>
        <v-list-item-subtitle class="text-right">{{ fs.total | prettyBytes }}</v-list-item-subtitle>
      </v-list-item>

      <v-list-item>
        <v-list-item-title>Free</v-list-item-title>
        <v-list-item-subtitle class="text-right">{{ fs.free | prettyBytes }}</v-list-item-subtitle>
      </v-list-item>
    </v-list>
  </v-card>
</template>

<script>
export default {
  data: () => ({
    wifiStats: ["ip", "gateway", "dns", "mac"]
  }),
  computed: {
    stats() {
      return this.$root._data.stats;
    },
    fsUsage() {
      return Math.round(
        (100 / this.stats.device.fs.total) * this.stats.device.fs.used
      );
    },
    fs() {
      return this.stats.device.fs;
    }
  }
};
</script>
