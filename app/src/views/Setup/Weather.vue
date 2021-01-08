<template>
  <setup-panel
    back
    @back="stepBack"
  >
    <template #icon>
      $wb_sunny
    </template>
    <template #headline>
      Weather
    </template>

    <v-text-field
      v-model="form.api"
      label="OpenWeatherMap API key"
    >
      <template #append-outer>
        <v-icon @click="registerApiKey()">
          $open_in_new
        </v-icon>
      </template>
    </v-text-field>

    <weather-find-location
      :api="form.api"
      :location.sync="form.location"
      :search.sync="form.name"
      :lang="lang"
      :unit="unit"
    />

    <template #actions>
      <v-btn
        :disabled="!isLocationValid"
        depressed
        block
        color="primary"
        @click="commitStep()"
      >
        Continue
      </v-btn>
      <v-btn
        class="ma-0 mt-3"
        text
        block
        color="primary"
        @click="nextStep()"
      >
        Set Up Later in Settings
      </v-btn>
    </template>
  </setup-panel>
</template>

<script>
  import { mapState, mapMutations, mapActions } from 'vuex'
  import weatherFindLocation from '@/components/WeatherFindLocation'
  import SetupPanel from '@/components/SetupPanel'

  export default {
    components: {
      SetupPanel,
      weatherFindLocation,
    },
    data: () => ({
      form: {
        api: '',
        name: '',
        location: 0,
      },
    }),
    computed: {
      ...mapState([
        'settings',
      ]),
      lang () {
        return this.settings.weather.language || 'EN'
      },
      unit () {
        return this.lang === 'EN' ? '' : 'metric'
      },
      isLocationValid () {
        return this.settings.weather.location > 0
      },
    },
    created () {
      this.resetChanges()
    },

    methods: {
      ...mapMutations(['updateSettings']),
      ...mapActions(['saveSettings']),

      commitStep () {
        this.updateSettings({
          weather: {
            api: this.form.api,
            name: this.form.name,
            location: this.form.location,
            lang: this.lang,
            unit: this.unit,
          },
        })

        this.nextStep()
      },
      resetChanges () {
        this.form.api = this.settings.weather.api
        this.form.location = this.settings.weather.location
        this.form.name = this.settings.weather.name
      },
      nextStep () {
        this.$router.push('/setup/name')
      },
      registerApiKey () {
        window.open('https://openweathermap.org/')
      },
      stepBack () {
        this.$router.push('/setup/wifi')
      },
    },
  }
</script>
