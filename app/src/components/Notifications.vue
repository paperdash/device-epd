<template>
  <v-snackbar
    v-model="snackbar"
    timeout="-1"
  >
    {{ notifications }}
  </v-snackbar>
</template>

<script>
  import { mapState, mapMutations } from 'vuex'

  export default {
    data: () => ({
      snackbar: false,
      timeoutHandler: null,
    }),
    computed: {
      ...mapState(['notifications']),
    },
    watch: {
      notifications (newVal) {
        if (newVal !== null) {
          this.snackbar = true
          this.resetTimeout()
        }
      },
    },
    methods: {
      ...mapMutations(['notification']),
      resetTimeout () {
        clearTimeout(this.timeoutHandler)

        this.timeoutHandler = setTimeout(() => {
          this.snackbar = false
          // clear notification
          this.notification(null)
        }, 3 * 1000)
      },
    },
  }
</script>

<style scoped>

</style>
