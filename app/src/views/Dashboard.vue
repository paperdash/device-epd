<template>
  <v-container fluid grid-list-md pa-2>
    <v-card class="mx-auto my-12" max-width="400" elevation="24">
      <!--<v-img class="device-screen-image" :aspect-ratio="16/9" src="/fs/screen.bmp"></v-img>-->
    </v-card>
    <v-btn class="ma-2" :loading="isLoading" color="secondary" @click="updateData">load weather data</v-btn>
  </v-container>
</template>

<script>
import apiDevice from "@/api/device";

export default {
  data: () => ({
    isLoading: false
  }),
  methods: {
    updateData() {
      this.isLoading = true;

      apiDevice.updateWeather(() => {
        this.isLoading = false;
      });
    }
  }
};
</script>

<style scoped>
</style>