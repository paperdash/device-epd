import Vue from 'vue'
import Vuetify from 'vuetify/lib'

// enable system icons
// @see https://vuetifyjs.com/en/customization/icons#component-icons
// @see https://github.com/vuetifyjs/vuetify/blob/master/packages/vuetify/src/services/icons/presets/mdi-svg.ts
const MY_ICONS = {
    // system icons
    // complete: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/check/baseline.svg')},
    // cancel: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/cancel/baseline.svg')},
    close: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/close/baseline.svg') },
    // delete: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/delete/baseline.svg')},
    clear: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/clear/baseline.svg') },
    success: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/check_circle/baseline.svg') },
    info: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/info/baseline.svg') },
    // warning: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/priority_high/baseline.svg')},
    error: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/warning/baseline.svg') },
    prev: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/chevron_left/baseline.svg') },
    next: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/chevron_right/baseline.svg') },
    // checkboxOn: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/check_box/baseline.svg')},
    // checkboxOff: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/check_box_outline_blank/baseline.svg')},
    // checkboxIndeterminate: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/indeterminate_check_box/baseline.svg')},
    // delimiter: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/fiber_manual_record/baseline.svg')}, // for carousel
    // sort: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/arrow_upward/baseline.svg')},
    // expand: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/keyboard_arrow_down/baseline.svg')},
    menu: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/menu/baseline.svg') },
    // subgroup: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/arrow_drop_down/baseline.svg')},
    dropdown: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/arrow_drop_down/baseline.svg') },
    radioOn: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/radio_button_checked/baseline.svg') },
    radioOff: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/radio_button_unchecked/baseline.svg') },
    // edit: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/edit/baseline.svg')},
    // ratingEmpty: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/star_border/baseline.svg')},
    // ratingFull: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/star/baseline.svg')},
    // ratingHalf: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/star_half/baseline.svg')},
    // loading: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/cached/baseline.svg')},
    // first: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/first_page/baseline.svg')},
    // last: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/last_page/baseline.svg')},
    // unfold: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/unfold_more/baseline.svg')},
    // file: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/attach_file/baseline.svg')},
    // plus: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/add/baseline.svg')},
    // minus: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/remove/baseline.svg')},

    // app icons
    // save: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/save/baseline.svg')},
    // more_vert: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/more_vert/baseline.svg')},
    // reorder: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/reorder/baseline.svg')},
    // preview: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/remove_red_eye/baseline.svg')},
    // search: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/search/baseline.svg')},
    memory: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/memory/baseline.svg') },
    lock: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/lock/baseline.svg') },
    settings: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/settings/baseline.svg') },
    storage: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/sd_storage/baseline.svg') },
    // playlist: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/live_tv/baseline.svg') },
    slideshow: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/slideshow/baseline.svg') },
    support: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/support/baseline.svg') },
    device: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/video_label/baseline.svg') },
    update: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/update/baseline.svg') },
    launch: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/launch/baseline.svg') },
    translate: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/translate/baseline.svg') },
    access_time: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/access_time/baseline.svg') },
    language: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/language/baseline.svg') },
    palette: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/palette/baseline.svg') },
    sentiment_satisfied_alt: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/sentiment_satisfied_alt/baseline.svg') },
    file: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/insert_drive_file/baseline.svg') },
    calendar_today: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/calendar_today/baseline.svg') },
    dashboard: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/dashboard/baseline.svg') },
    done: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/done/baseline.svg') },
    link: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/link/baseline.svg') },
    help: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/help/baseline.svg') },
    vpn_key: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/vpn_key/baseline.svg') },
    place: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/place/baseline.svg') },
    monochrome_photos: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/monochrome_photos/baseline.svg') },

    // wifi
    wifi: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/wifi/baseline.svg') },
    signalWifiOff: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/signal_wifi_off/baseline.svg') },
    signalWifi0: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/signal_wifi_0_bar/baseline.svg') },
    signalWifi1: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/signal_wifi_1_bar/baseline.svg') },
    signalWifi1Lock: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/signal_wifi_1_bar_lock/baseline.svg') },
    signalWifi2: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/signal_wifi_2_bar/baseline.svg') },
    signalWifi2Lock: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/signal_wifi_2_bar_lock/baseline.svg') },
    signalWifi3: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/signal_wifi_3_bar/baseline.svg') },
    signalWifi3Lock: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/signal_wifi_3_bar_lock/baseline.svg') },
    signalWifi4: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/signal_wifi_4_bar/baseline.svg') },
    signalWifi4Lock: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/signal_wifi_4_bar_lock/baseline.svg') },

    // setup
    wb_sunny: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/wb_sunny/baseline.svg') },
    open_in_new: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/open_in_new/baseline.svg') },
    face: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/face/baseline.svg') },
    autorenew: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/autorenew/baseline.svg') },
    check: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/check/baseline.svg') },
    visibility: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/visibility/baseline.svg') },
    visibility_off: { component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/visibility_off/baseline.svg') },
}

Vue.use(Vuetify)

const opts = {
    icons: {
        iconfont: '',
        values: MY_ICONS,
    },
}

export default new Vuetify(opts)
