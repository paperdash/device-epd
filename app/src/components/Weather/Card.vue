<template>
  <v-card
    outlined_
    flat
  >
    <v-skeleton-loader
      type="card-heading, list-item-three-line, list-item-avatar, list-item-avatar, list-item-avatar, list-item-avatar"
      :loading="isLoading"
    >
      <v-list-item two-line>
        <v-list-item-content v-if="weather">
          <v-list-item-title class="text-h5">
            {{ weather.name }}
          </v-list-item-title>
          <v-list-item-subtitle>{{ new Date(weather.dt *1000).toLocaleString(undefined, {weekday:'short', hour: 'numeric'}) }}, {{ weather.weather[0].description }}</v-list-item-subtitle>
        </v-list-item-content>
      </v-list-item>

      <template v-if="weather">
        <v-card-text>
          <v-row align="center">
            <v-col
              class="text-h2"
              cols="6"
            >
              {{ currentTemp }} &deg;C
            </v-col>
            <v-col cols="6">
              <v-img
                :src="currentConditionIcon"
                width="92"
              />
            </v-col>
          </v-row>
        </v-card-text>

        <v-list-item>
          <v-list-item-icon>
            <v-icon>mdi-send</v-icon>
          </v-list-item-icon>
          <v-list-item-subtitle>{{ weather.wind.speed | round }} km/h</v-list-item-subtitle>
        </v-list-item>

        <v-list-item>
          <v-list-item-icon>
            <v-icon>mdi-cloud-download</v-icon>
          </v-list-item-icon>
          <v-list-item-subtitle>{{ weather.main.humidity }}%</v-list-item-subtitle>
        </v-list-item>

        <v-divider class="mx-4" />

        <template v-if="forecast">
          <v-list class="transparent">
            <v-list-item
              v-for="item in forecast.list"
              :key="item.dt"
            >
              <v-list-item-title>{{ new Date(item.dt *1000).toLocaleString(undefined, {weekday:'long'}) }}</v-list-item-title>

              <v-list-item-icon class="ma-0">
                <v-img :src="getConditionIcon(item.weather[0].icon)" />
              </v-list-item-icon>

              <v-list-item-subtitle
                class="text-right"
              >
                {{ item.temp.max | round }}&deg; / {{ item.temp.min | round }}&deg;
              </v-list-item-subtitle>
            </v-list-item>
          </v-list>
        </template>
      </template>

      <v-divider />

      <v-card-actions>
        <v-spacer />
        <v-btn
          :loading="isUpdatingData"
          depressed
          @click="updateData"
        >
          <v-icon left>
            $update
          </v-icon>
          Update Now
        </v-btn>
      </v-card-actions>
    </v-skeleton-loader>
  </v-card>
</template>

<script>
  import weatherApi from '@/api/weather'

  export default {
    data: () => ({
      isLoading: true,
      isUpdatingData: false,

      weather: null,
      forecast: null,
    }),

    computed: {
      currentTemp () {
        return Math.round(this.weather.main.temp)
      },
      currentConditionIcon () {
        return (
          'http://openweathermap.org/img/wn/' +
          this.weather.weather[0].icon +
          '@2x.png'
        )
      },
    },
    created () {
      this.loadData()
    },
    methods: {
      getConditionIcon (code) {
        return 'http://openweathermap.org/img/wn/' + code + '@2x.png'
      },
      loadData () {
        this.isLoading = true
        weatherApi.getCurrent(weather => {
          this.weather = weather

          weatherApi.getForecast(forecast => {
            this.forecast = forecast

            this.isLoading = false
          })
        })
      },
      updateData () {
        this.isUpdatingData = true

        weatherApi.updateWeather(() => {
          // TODO trigger esp weather data reload
          this.isUpdatingData = false

          this.loadData()
        })
      },
    },
  }
</script>
