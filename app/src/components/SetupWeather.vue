<template>
  <v-card flat>
    <v-card-text>
      <v-text-field
        v-model="settings.weather.api"
        label="i8n:OpenWeatherMap API key"
        placeholder="###"
      />

      <weather-find-location
        :api="settings.weather.api"
        :location.sync="settings.weather.location"
        :lang="lang"
        :unit="unit"
      />
    </v-card-text>
  </v-card>
</template>

<script>
  import weatherFindLocation from '@/components/WeatherFindLocation'

  export default {
    components: {
      weatherFindLocation,
    },
    props: {
      settings: {
        type: Object,
        required: true,
      },
    },
    data: () => ({
    }),
    computed: {
      lang () {
        return this.settings.language || 'EN'
      },
      unit () {
        return this.settings.language === 'EN' ? '' : 'metric'
      },
    },
    methods: {
      commit () {},
    },
  }
</script>
