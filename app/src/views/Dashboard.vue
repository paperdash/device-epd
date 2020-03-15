<template>
  <v-container fluid _grid-list-md _pa-2>
    <v-row>
      <v-col cols="12" md="6">
        <screen-card></screen-card>
      </v-col>
      <v-col cols="12" sm="6" md="4">
        <weather-card></weather-card>
      </v-col>
      <v-col cols="12" sm="6" md="4">
        <device-card></device-card>
      </v-col>
    </v-row>
  </v-container>
</template>

<script>
import screenCard from "@/components/ScreenCard";
import weatherCard from "@/components/WeatherCard";
import deviceCard from "@/components/DeviceCard";

export default {
  components: {
    screenCard,
    weatherCard,
    deviceCard
  },
  data: () => ({
    isLoading: false
  })
};
</script>

<style scoped>
</style>