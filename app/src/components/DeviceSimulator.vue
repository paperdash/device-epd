<template>
  <svg-case
    id="device"
    :class="cssClass"
  />
</template>

<script>
  const SvgCase = () => import('@/assets/case_editor.svg?inline')

  export default {
    name: 'DeviceSimulator',
    components: { SvgCase },
    props: {
      theme: {
        type: String,
        default: 'black',
        validator (value) {
          return ['black', 'white'].includes(value)
        },
      },
      face: {
        type: String,
        default: 'calendar',
        validator (value) {
          return ['weather', 'calendar'].includes(value)
        },
      },
      front: {
        type: String,
        default: 'black',
        validator (value) {
          return ['black', 'white', 'orange'].includes(value)
        },
      },
      frame: {
        type: String,
        default: 'black',
        validator (value) {
          return ['black', 'white', 'orange'].includes(value)
        },
      },
    },
    // data: () => ({}),
    computed: {
      cssClass () {
        return [
          'theme_' + this.theme,
          'face_' + this.face,
          'front_' + this.front,
          'frame_' + this.frame,
        ]
      },
    },
  }
</script>

<style scoped>
#device * {
  transition: all 0.2s ease-out;
}
#device #border {
  fill: #343434;
}

#device.theme_white.face_weather #image {
  fill: url(#pattern_w_weather);
}
#device.theme_black.face_weather #image {
  fill: url(#pattern_b_weather);
}
#device.theme_white.face_calendar #image {
  fill: url(#pattern_w_calendar);
}
#device.theme_black.face_calendar #image {
  fill: url(#pattern_b_calendar);
}

/* front color */
#device.front_black #front {
  fill: #000;
}
#device.front_white #front {
  fill: #f3f3f3;
}
#device.front_orange #front {
  fill: #ee8816;
}

/* case color */
#device.frame_black #case {
  fill: #000;
}
#device.frame_white #case {
  fill: #f3f3f3;
}
#device.frame_orange #case {
  fill: #ee8816;
}
</style>
