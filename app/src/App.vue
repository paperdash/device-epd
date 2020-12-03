<template>
  <v-app>
    <template v-if="isLoading">
      <v-overlay
        :absolute="true"
        :value="true"
      >
        <v-progress-circular
          indeterminate
          size="64"
        />
      </v-overlay>
    </template>
    <template v-else>
      <component :is="layout" />
    </template>
  </v-app>
</template>

<script>
  import { mapState, mapActions } from 'vuex'
  import notifications from '@/components/Notifications'
  import '@/assets/app.css'

  export default {
    components: {
      // eslint-disable-next-line vue/no-unused-components
      'layout-default': () => import('@/layouts/default'),
      // eslint-disable-next-line vue/no-unused-components
      'layout-setup': () => import('@/layouts/setup'),
      notifications,
    },
    data: () => ({
      isLoading: true,
    }),
    computed: {
      ...mapState(['stats']),
      layout () {
        return 'layout-' + (this.$route.meta.layout || 'default')
      },
    },
    created () {
      // load
      Promise.all([this.loadStats(), this.loadSettings()]).then(() => {
        this.isLoading = false
      })
    },
    methods: {
      ...mapActions([
        'loadStats',
        'loadSettings',
      ]),
    },
  }
</script>

<style scoped>
</style>
