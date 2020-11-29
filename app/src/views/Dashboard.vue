<template>
  <v-container
    class="_grid-list-md _pa-2"
    fluid
  >
    <weather-card />

    - update screen in<br>
    - update weather in<br>
    - show api infos and short how to

    <v-row v-if="0">
      <v-col
        cols="12"
        md="6"
      >
        <!--
        <v-chip
          outlined
          :title="firmware.rev"
          color="text--disabled"
        >
          {{ (new Date(firmware.created * 1000).toLocaleDateString()) }}
          @
          {{ firmware.rev.substring(0,8) }}
        </v-chip>
        -->
      </v-col>
      <v-col
        cols="12"
        sm="6"
        md="4"
      >
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
    computed: {
      firmware () {
        return {
          // eslint-disable-next-line no-undef
          created: __BUILD_TIME__ || JSON.stringify(new Date().getTime() / 1000 | 0),
          // eslint-disable-next-line no-undef
          rev: __COMMIT_HASH__ || 'dev-master',
        }
      },
    },
  }
</script>

<style scoped>
</style>
