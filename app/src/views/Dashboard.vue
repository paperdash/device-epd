<template>
  <v-container
    class="_grid-list-md _pa-2"
    fluid
  >
    <weather-card />
  </v-container>
</template>

<script>
  import weatherCard from '@/components/Weather/Card'

  export default {
    components: {
      weatherCard,
    },
    data: () => ({
      isLoading: false,
    }),
  }
</script>

<style scoped>
</style>
