<template>
  <v-main>
    <transition-page>
      <router-view />
    </transition-page>
  </v-main>
</template>

<script>
  import TransitionPage from '@/components/TransitionPage'
  export default {
    name: 'Setup',
    components: { TransitionPage },
  }
</script>

<style scoped>

</style>
