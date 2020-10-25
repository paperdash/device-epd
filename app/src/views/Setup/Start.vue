<template>
  <v-container
    class="_fill-height"
    fluid
  >
    <v-row
      no-gutters
      justify="center"
    >
      <v-col
        lg="5"
        md="6"
        sm="8"
      >
        <v-card flat>
          <v-card-title class="display-2 mt-12 justify-center text-center">
            Hello paperdash
          </v-card-title>

          <Case
            v-if="0"
            id="device_"
            :class="[device.theme, device.case, device.front, 'case_orange', 'my-12']"
          />

          <svg
            id="device"
            :class="[device.theme, device.case, device.front, 'case_orange front_orange', 'my-12 mx-auto']"
            version="1.0"
            xmlns="http://www.w3.org/2000/svg"
            _width="561px"
            _height="527px"
            viewBox="0 0 5610 5270"
            preserveAspectRatio="xMidYMid meet"
            width="400"
          >

            <g
              id="border"
              fill="#262626"
              stroke="none"
            >
              <path
                d="M442 5110 c-227 -86 -417 -161 -422 -166 -4 -5 -12 -1063 -16 -2350 -7 -2292 -6 -2342 12 -2352 31 -16 817 -242 844 -242 14 0 1086 158 2383 352 l2358 353 6 25 c9 35 -66 3683 -76 3693 -4 4 -1049 194 -2322 422 -1273 228 -2323 416 -2334 419 -11 2 -206 -67 -433 -154z"
              />
            </g>
            <g
              fill="#f3f3f3"
              stroke="none"
            >
              <path
                id="front"
                d="M860 2630 l0 -2611 33 6 c17 3 1080 163 2360 355 1281 191 2330 350 2332 352 6 6 -70 3666 -76 3672 -4 4 -4627 836 -4645 836 -2 0 -4 -1175 -4 -2610z m2378 1721 c1217 -175 2214 -320 2216 -322 7 -6 69 -3179 63 -3185 -8 -9 -4489 -624 -4499 -618 -10 6 -11 4444 0 4444 4 0 1003 -144 2220 -319z m2249 2 c-4 -3 -7 0 -7 7 0 7 3 10 7 7 3 -4 3 -10 0 -14z m60 -2890 c-4 -3 -7 0 -7 7 0 7 3 10 7 7 3 -4 3 -10 0 -14z"
              />
              <path
                id="image"
                d="M1022 4642 c-7 -19 5 -4393 13 -4400 5 -5 4465 605 4472 611 4 4 -59 3161 -63 3166 -3 3 -3200 465 -4387 634 -19 2 -31 -1 -35 -11z"
              />
              <path
                id="case"
                d="M438 5081 c-214 -81 -390 -150 -393 -153 -7 -6 -28 -4658 -22 -4664 2 -2 182 -56 400 -119 218 -64 403 -118 412 -121 13 -6 15 259 15 2600 0 2078 -3 2606 -12 2605 -7 0 -187 -67 -400 -148z"
              />
            </g>
          </svg>

          <v-card-actions>
            <v-btn
              depressed
              block
              color="primary"
              @click="commitStep()"
            >
              Continue
            </v-btn>
          </v-card-actions>
        </v-card>
      </v-col>
    </v-row>
  </v-container>
</template>

<script>
  export default {
    data: () => ({
      device: {
        theme: 'theme_w',
        case: 'case_whitey',
        front: 'front_whitey',
      },
    }),
    methods: {
      commitStep () {
        // TODO sav
        this.$router.push('/setup/country')
      },
    },
  }
</script>

<style scoped>
#device #border,
#device > path {
  fill: #343434;
}

/* TODO device theme */
#device.theme_w #image {
  fill: url(#pattern_w);
}
#device.theme_b #image {
  fill: url(#pattern_b);
}

/* front color */
#device.front_blacky #front {
  fill: #000;
}
#device.front_whitey #front {
  fill: #f3f3f3;
}
#device.front_orange #front {
  fill: #ee8816;
}

/* case color */
#device.case_blacky #case {
  fill: #000;
}
#device.case_whitey #case {
  fill: #f3f3f3;
}
#device.case_orange #case {
  fill: #ee8816;
}
</style>
