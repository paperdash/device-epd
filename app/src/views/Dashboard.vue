<template>
  <v-container
    class="_grid-list-md _pa-2"
    fluid
  >
    <v-row>
      <v-col
        cols="12"
        md="6"
      >
        <screen-card />
      </v-col>
      <v-col
        cols="12"
        sm="6"
        md="4"
      >
        <weather-card />
        <device-card class="mt-5" />
      </v-col>
    </v-row>
  </v-container>
</template>

<script>
  import screenCard from '@/components/ScreenCard'
  import weatherCard from '@/components/WeatherCard'
  import deviceCard from '@/components/DeviceCard'

  export default {
    components: {
      screenCard,
      weatherCard,
      deviceCard,
    },
    data: () => ({
      isLoading: false,
    }),
  }
</script>

<style scoped>
</style>
