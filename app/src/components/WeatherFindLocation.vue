<template>
  <v-autocomplete
    v-model="model"
    :disabled="!api"
    :items="entries"
    :loading="isLoading"
    :search-input.sync="search"
    no-filter
    _hide-no-data
    item-text="name"
    item-value="id"
    label="Location"
    placeholder="Start typing to Search"
    prepend-icon="$place"
  >
    <template #item="{ item }">
      <v-list-item-avatar
        v-if="item.weather"
        color="grey lighten-2"
        class="headline font-weight-light white--text"
      >
        <img
          width="64"
          :src="getConditionIcon(item.weather[0].icon)"
        >
      </v-list-item-avatar>

      <v-list-item-content>
        <v-list-item-title>
          {{ item.name }}, {{ item.sys.country }}
        </v-list-item-title>
        <v-list-item-subtitle v-if="item.weather">
          <img
            width="16"
            :src="getCountryFlag(item.sys.country)"
          >
          {{ item.weather[0].description }}
        </v-list-item-subtitle>
      </v-list-item-content>
      <v-list-item-action v-if="item.main">
        {{ item.main.temp }}°С
      </v-list-item-action>
    </template>
    <template #selection="{ item }">
      {{ item.name }}, {{ item.sys.country }}
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
      search: null,
    }),
    watch: {
      model (val) {
        this.$emit('update:location', val)
      },
      search (val) {
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
            this.entries = res.data.list
            this.isLoading = false
          })
      },
    },
    created () {
      // TODO
      this.entries = [{
        name: 'Freilassing',
        sys: {
          country: 'DE',
        },
        id: this.location,
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
