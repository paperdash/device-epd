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
      layout: 'layout-default',
    }),
    computed: {
      ...mapState(['stats']),
    },
    created () {
      // switch layout if necessary
      this.$router.beforeEach((to, from, next) => {
        const layout = to.meta.layout || 'default'
        this.layout = 'layout-' + layout
        next()
      })

      // load

      Promise.all([this.loadStats(), this.loadSettings()]).then((values) => {
        this.isLoading = false
      })

      /*
      this.loadStats().then(() => {
        this.isLoading = false
      })
       */
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
