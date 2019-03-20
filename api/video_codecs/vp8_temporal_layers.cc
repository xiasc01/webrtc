/*
 *  Copyright (c) 2019 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "api/video_codecs/vp8_temporal_layers.h"

#include <utility>

#include "rtc_base/checks.h"

namespace webrtc {

Vp8TemporalLayers::Vp8TemporalLayers(
    std::vector<std::unique_ptr<Vp8FrameBufferController>>&& controllers)
    : controllers_(std::move(controllers)) {
  RTC_DCHECK(!controllers_.empty());
  RTC_DCHECK(std::none_of(
      controllers_.begin(), controllers_.end(),
      [](const std::unique_ptr<Vp8FrameBufferController>& controller) {
        return controller.get() == nullptr;
      }));
}

size_t Vp8TemporalLayers::StreamCount() const {
  return controllers_.size();
}

bool Vp8TemporalLayers::SupportsEncoderFrameDropping(
    size_t stream_index) const {
  RTC_DCHECK_LT(stream_index, controllers_.size());
  return controllers_[stream_index]->SupportsEncoderFrameDropping(0);
}

void Vp8TemporalLayers::OnRatesUpdated(
    size_t stream_index,
    const std::vector<uint32_t>& bitrates_bps,
    int framerate_fps) {
  RTC_DCHECK_LT(stream_index, controllers_.size());
  return controllers_[stream_index]->OnRatesUpdated(0, bitrates_bps,
                                                    framerate_fps);
}

bool Vp8TemporalLayers::UpdateConfiguration(size_t stream_index,
                                            Vp8EncoderConfig* cfg) {
  RTC_DCHECK_LT(stream_index, controllers_.size());
  return controllers_[stream_index]->UpdateConfiguration(0, cfg);
}

Vp8FrameConfig Vp8TemporalLayers::UpdateLayerConfig(size_t stream_index,
                                                    uint32_t rtp_timestamp) {
  RTC_DCHECK_LT(stream_index, controllers_.size());
  return controllers_[stream_index]->UpdateLayerConfig(0, rtp_timestamp);
}

void Vp8TemporalLayers::OnEncodeDone(size_t stream_index,
                                     uint32_t rtp_timestamp,
                                     size_t size_bytes,
                                     bool is_keyframe,
                                     int qp,
                                     CodecSpecificInfo* info) {
  RTC_DCHECK_LT(stream_index, controllers_.size());
  return controllers_[stream_index]->OnEncodeDone(0, rtp_timestamp, size_bytes,
                                                  is_keyframe, qp, info);
}

void Vp8TemporalLayers::OnPacketLossRateUpdate(float packet_loss_rate) {
  for (auto& controller : controllers_) {
    controller->OnPacketLossRateUpdate(packet_loss_rate);
  }
}

void Vp8TemporalLayers::OnRttUpdate(int64_t rtt_ms) {
  for (auto& controller : controllers_) {
    controller->OnRttUpdate(rtt_ms);
  }
}

}  // namespace webrtc