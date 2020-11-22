<template>
  <div class="pa-5">
    <v-card
      flat
      width="400"
      class="mx-auto"
    >
      <v-card-title class="display-2 mb-12 justify-center text-center">
        Weather settings
      </v-card-title>

      <v-card-text>
        <v-text-field
          v-model="settings.weather.api"
          label="OpenWeatherMap API key"
        >
          <template #append-outer>
            <v-btn
              icon
              href="https://openweathermap.org/"
              target="_blank"
            >
              <v-icon>$launch</v-icon>
            </v-btn>
          </template>
        </v-text-field>

        <weather-find-location
          :api="settings.weather.api"
          :location.sync="settings.weather.location"
          :lang="settings.weather.lang"
          :unit="settings.weather.unit"
        />

        <v-select
          v-model="settings.weather.lang"
          :disabled="!settings.weather.api"
          :items="weatherLang"
          label="Language"
        />
        <v-select
          v-model="settings.weather.unit"
          :disabled="!settings.weather.api"
          :items="weatherUnit"
          label="Units"
        />
      </v-card-text>

      <v-divider class="mt-12" />
      <v-card-actions>
        <v-btn text>
          Cancel
        </v-btn>
        <v-spacer />
        <v-btn
          color="primary"
          text
          @click="commitChanges"
        >
          Submit
        </v-btn>
      </v-card-actions>
    </v-card>
  </div>
</template>

<script>
  import { mapState, mapGetters, mapActions } from 'vuex'
  import WeatherFindLocation from '@/components/WeatherFindLocation'
  export default {
    components: {
      WeatherFindLocation,
    },
    data: () => ({
      // @see https://openweathermap.org/current#multi
      weatherLang: [
        { text: 'English', value: 'en' },
        { text: 'Deutsch', value: 'de' },
      ],
      weatherUnit: [
        { text: 'Imperial', value: '' },
        { text: 'Metrisch', value: 'metric' },
      ],
    }),
    computed: {
      ...mapState([
        'stats',
        'settings',
      ]),
      ...mapGetters([
      ]),
    },
    methods: {
      ...mapActions(['saveSettings']),
      commitChanges () {
        this.saveSettings()
      },
    },
  }
</script>

<style scoped>
</style>
