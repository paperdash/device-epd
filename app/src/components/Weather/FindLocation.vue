<template>
  <v-autocomplete
    v-model="model"
    :disabled="!api"
    :items="entries"
    :loading="isLoading"
    :search-input.sync="searchInput"
    no-filter
    hide-no-data
    item-text="name"
    item-value="id"
    label="Location"
    placeholder="Start typing to Search"
    prepend-icon="$place"
    return-object
    clearable
  >
    <template #item="{ item }">
      <v-list-item-avatar
        v-if="item.weather"
        color="grey lighten-2"
        class="headline font-weight-light white--text"
      >
        <img
          width="64"
          :src="getConditionIcon(item.weather.icon)"
        >
      </v-list-item-avatar>

      <v-list-item-content>
        <v-list-item-title>
          {{ item.label }}
        </v-list-item-title>
        <v-list-item-subtitle v-if="item.weather">
          <img
            width="16"
            :src="getCountryFlag(item.country)"
          >
          {{ item.weather.description }}
        </v-list-item-subtitle>
      </v-list-item-content>
      <v-list-item-action v-if="item.main">
        {{ item.temp }}°С
      </v-list-item-action>
    </template>
    <template #selection="{ item }">
      {{ item.label }}
    </template>
  </v-autocomplete>
</template>

<script>
  import axios from 'axios'

  export default {
    props: {
      location: {
        type: Number,
        required: false,
      },
      api: {
        type: String,
        required: true,
      },
      search: {
        type: String,
        required: false,
      },
      lang: {
        type: String,
        required: true,
      },
      unit: {
        type: String,
        required: true,
      },
    },

    data: () => ({
      entries: [],
      isLoading: false,
      model: null,
      searchInput: null,
    }),
    watch: {
      model (item) {
        if (item) {
          this.$emit('update:location', item.id)
          this.$emit('update:search', item.label)
        }
      },
      searchInput (val) {
        if (!val || val.length < 3) {
          return
        }

        // Items have already been requested
        if (this.isLoading) {
          return
        }

        this.isLoading = true
        this.entries = []

        // search
        let url = 'https://api.openweathermap.org/data/2.5/find?appid='
        url += this.api
        url += '&lang=' + this.lang
        url += '&units=' + this.unit
        url += '&type=like&sort=population&cnt=10'
        url += '&q=' + val

        axios.get(url)
          .then(res => {
            const entries = []

            res.data.list.forEach((item) => {
              entries.push({
                id: item.id,
                label: item.name + ', ' + item.sys.country,
                temp: item.main.temp,
                country: item.sys.country,
                weather: {
                  icon: item.weather[0].icon,
                  description: item.weather[0].description,
                },
              })
            })

            this.entries = entries
            this.isLoading = false
          })
      },
    },
    created () {
      // create entry for the current item
      this.entries = [{
        id: this.location,
        label: this.search,
      }]
      this.model = this.location
    },
    methods: {
      getCountryFlag (code) {
        return (
          'http://openweathermap.org/images/flags/' + code.toLowerCase() + '.png'
        )
      },
      getConditionIcon (code) {
        return 'http://openweathermap.org/img/wn/' + code + '@2x.png'
      },
    },
  }
</script>
