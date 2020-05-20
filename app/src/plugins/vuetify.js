import Vue from 'vue'
import Vuetify from 'vuetify/lib'

// enable system icons
// @see https://vuetifyjs.com/en/customization/icons#component-icons
// @see https://github.com/vuetifyjs/vuetify/blob/master/packages/vuetify/src/services/icons/presets/mdi-svg.ts
const MY_ICONS = {
    // system icons
    //complete: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/check/baseline.svg')},
    //cancel: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/cancel/baseline.svg')},
    close: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/close/baseline.svg')},
    //delete: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/delete/baseline.svg')},
    //clear: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/clear/baseline.svg')},
    //success: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/check_circle/baseline.svg')},
    //info: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/info/baseline.svg')},
    //warning: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/priority_high/baseline.svg')},
    //error: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/warning/baseline.svg')},
    prev: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/chevron_left/baseline.svg')},
    next: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/chevron_right/baseline.svg')},
    //checkboxOn: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/check_box/baseline.svg')},
    //checkboxOff: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/check_box_outline_blank/baseline.svg')},
    //checkboxIndeterminate: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/indeterminate_check_box/baseline.svg')},
    //delimiter: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/fiber_manual_record/baseline.svg')}, // for carousel
    //sort: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/arrow_upward/baseline.svg')},
    //expand: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/keyboard_arrow_down/baseline.svg')},
    //menu: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/menu/baseline.svg')},
    //subgroup: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/arrow_drop_down/baseline.svg')},
    dropdown: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/arrow_drop_down/baseline.svg')},
    radioOn: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/radio_button_checked/baseline.svg')},
    radioOff: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/radio_button_unchecked/baseline.svg')},
    //edit: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/edit/baseline.svg')},
    //ratingEmpty: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/star_border/baseline.svg')},
    //ratingFull: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/star/baseline.svg')},
    //ratingHalf: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/star_half/baseline.svg')},
    //loading: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/cached/baseline.svg')},
    //first: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/first_page/baseline.svg')},
    //last: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/last_page/baseline.svg')},
    //unfold: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/unfold_more/baseline.svg')},
    //file: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/attach_file/baseline.svg')},
    //plus: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/add/baseline.svg')},
    //minus: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/remove/baseline.svg')},

    // app icons
    //save: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/save/baseline.svg')},
    //more_vert: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/more_vert/baseline.svg')},
    //reorder: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/reorder/baseline.svg')},
    //preview: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/remove_red_eye/baseline.svg')},
	//search: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/search/baseline.svg')},
	memory: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/memory/baseline.svg')},
	lock: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/lock/baseline.svg')},
	settings: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/settings/baseline.svg')},

	// wifi
	signalWifiOff: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/signal_wifi_off/baseline.svg')},
	signalWifi0: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/signal_wifi_0_bar/baseline.svg')},
	signalWifi1: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/signal_wifi_1_bar/baseline.svg')},
	signalWifi1Lock: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/signal_wifi_1_bar_lock/baseline.svg')},
	signalWifi2: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/signal_wifi_2_bar/baseline.svg')},
	signalWifi2Lock: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/signal_wifi_2_bar_lock/baseline.svg')},
	signalWifi3: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/signal_wifi_3_bar/baseline.svg')},
	signalWifi3Lock: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/signal_wifi_3_bar_lock/baseline.svg')},
	signalWifi4: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/signal_wifi_4_bar/baseline.svg')},
	signalWifi4Lock: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/signal_wifi_4_bar_lock/baseline.svg')},

	// setup
	wb_sunny: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/wb_sunny/baseline.svg')},
	open_in_new: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/open_in_new/baseline.svg')},
	face: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/face/baseline.svg')},
	autorenew: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/autorenew/baseline.svg')},
	check: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/check/baseline.svg')},
	visibility: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/visibility/baseline.svg')},
	visibility_off: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/visibility_off/baseline.svg')}
}


Vue.use(Vuetify)

// usage: {{ file.size | prettyBytes }}
Vue.filter('prettyBytes', function (num) {
	// jacked from: https://github.com/sindresorhus/pretty-bytes
	if (typeof num !== 'number' || isNaN(num)) {
		throw new TypeError('Expected a number');
	}

	var exponent;
	var unit;
	var neg = num < 0;
	var units = ['B', 'kB', 'MB', 'GB', 'TB', 'PB', 'EB', 'ZB', 'YB'];

	if (neg) {
		num = -num;
	}

	if (num < 1) {
		return (neg ? '-' : '') + num + ' B';
	}

	exponent = Math.min(Math.floor(Math.log(num) / Math.log(1000)), units.length - 1);
	num = (num / Math.pow(1000, exponent)).toFixed(2) * 1;
	unit = units[exponent];

	return (neg ? '-' : '') + num + ' ' + unit;
});

/**
 * Vue filter to round the decimal to the given place.
 * http://jsfiddle.net/bryan_k/3ova17y9/
 *
 * @param {String} value    The value string.
 * @param {Number} decimals The number of decimal places.
 */
Vue.filter('round', function (value, decimals) {
	if (!value) {
		value = 0;
	}

	if (!decimals) {
		decimals = 0;
	}

	value = Math.round(value * Math.pow(10, decimals)) / Math.pow(10, decimals);
	return value;
});


/**
 * usage: {{ wifi.rssi | wifiIcon(wifi.secure) }}
 * @param {Number} rssi    rssi value
 * @param {Boolean} secure Is wifi protected
*/
Vue.filter('wifiIcon', function (rssi, secure) {
	let icon = '$signalWifi'

	// strength
	if (rssi >= -67) {
		icon += 4
	}
	else if (rssi >= -70) {
		icon += 3
	}
	else if (rssi >= -80) {
		icon += 2
	}
	else if (rssi >= -90) {
		icon += 1
	}
	else {
		icon += 0
	}

	// secure
	if (secure !== 0 && rssi >= -90) {
		icon += 'Lock'
	}

	return icon
});

const opts = {
    icons: {
        iconfont: '',
        values: MY_ICONS
    }
}

export default new Vuetify(opts)