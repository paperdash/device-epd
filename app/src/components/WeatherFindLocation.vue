<template>
  <v-autocomplete
    ref="input"
    v-model="model"
    :disabled="!api"
    :items="entries"
    :loading="isLoading"
    :search-input.sync="search"
    _hide-no-data
    _hide-selected
    item-text="name"
    item-value="id"
    label="Location"
    placeholder="Start typing to Search"
    _return-object
  >
    <template #item="{ item }">
      <v-list-item-avatar
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
        <v-list-item-subtitle>
          <img
            width="16"
            :src="getCountryFlag(item.sys.country)"
          >
          {{ item.weather[0].description }}
        </v-list-item-subtitle>
      </v-list-item-content>
      <v-list-item-action>
        {{ item.main.temp }}°С
      </v-list-item-action>

      <v-list-item
        v-if="0"
        two-line
        class="pa-0"
      >
        <v-list-item-icon class="ma-0">
          <img
            width="64"
            :src="getConditionIcon(item.weather[0].icon)"
          >
        </v-list-item-icon>

        <v-list-item-content>
          <v-list-item-title>
            {{ item.name }}, {{ item.sys.country }}
            <img :src="getCountryFlag(item.sys.country)">
            <strong>{{ item.weather[0].description }}</strong>
          </v-list-item-title>
          <v-list-item-subtitle>
            <v-chip
              class="font-weight-bold"
              small
            >
              {{ item.main.temp }}°С
            </v-chip>
          </v-list-item-subtitle>
        </v-list-item-content>
      </v-list-item>
    </template>
  </v-autocomplete>
</template>

<script>
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
        if (!val || val.length < 3) return

        // Items have already been requested
        if (this.isLoading) return

        this.isLoading = true

        // search
        let url = 'https://api.openweathermap.org/data/2.5/find?appid='
        url += this.api
        url += '&lang=' + this.lang
        url += '&units=' + this.unit
        url += '&type=like&sort=population&cnt=10'
        url += '&q=' + val

        fetch(url)
          .then(res => res.json())
          .then(res => {
            this.count = res.count
            this.entries = res.list
          })
          .catch(err => {
            console.log(err)
          })
          .finally(() => (this.isLoading = false))
      },
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
