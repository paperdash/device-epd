<template>
  <v-card
    flat
    rounded="lg"
  >
    <v-card-title class="display-2 mb-12 justify-center text-center">
      Weather settings
    </v-card-title>

    <v-card-text>
      <v-text-field
        v-model="form.api"
        label="OpenWeatherMap API key"
        prepend-icon="$vpn_key"
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
        :api="form.api"
        :location.sync="form.location"
        :search.sync="form.name"
        :lang="form.lang"
        :unit="form.unit"
      />

      <v-select
        v-model="form.unit"
        :disabled="!form.api"
        :items="weatherUnit"
        label="Units"
        prepend-icon=" "
      />
      <v-select
        v-model="form.lang"
        :disabled="!form.api"
        :items="weatherLang"
        label="Language"
        prepend-icon="$translate"
      />
    </v-card-text>

    <v-divider class="mt-12" />
    <v-card-actions>
      <v-btn
        text
        @click="resetChanges"
      >
        Restore
      </v-btn>
      <v-spacer />
      <v-btn
        :loading="isProcessing"
        depressed
        @click="commitChanges"
      >
        <v-icon left>
          $done
        </v-icon>
        Save
      </v-btn>
    </v-card-actions>
  </v-card>
</template>

<script>
  import { mapState, mapActions, mapMutations } from 'vuex'
  import WeatherFindLocation from '@/components/WeatherFindLocation'
  export default {
    components: {
      WeatherFindLocation,
    },
    data: () => ({
      isProcessing: false,
      form: {
        api: '',
        location: '',
        name: '',
        lang: '',
        unit: '',
      },

      // @see https://openweathermap.org/current#multi
      weatherLang: [
        { text: 'Use system settings', value: '' },
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
    },
    created () {
      this.resetChanges()
    },

    methods: {
      ...mapMutations(['updateSettings']),
      ...mapActions(['saveSettings']),
      commitChanges () {
        this.isProcessing = true

        this.updateSettings({
          weather: {
            api: this.form.api,
            location: this.form.location,
            lang: this.form.lang,
            unit: this.form.unit,
            name: this.form.name,
          },
        })

        this.saveSettings().then(() => {
          this.isProcessing = false
        })
      },
      resetChanges () {
        this.form.api = this.settings.weather.api
        this.form.location = this.settings.weather.location
        this.form.name = this.settings.weather.name
        this.form.lang = this.settings.weather.lang
        this.form.unit = this.settings.weather.unit
      },
    },
  }
</script>

<style scoped>
</style>
