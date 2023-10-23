#pragma once
#include "Math.Pose.h"

namespace nar {
  inline XrReferenceSpaceCreateInfo
  GetXrReferenceSpaceCreateInfo(const std::string &referenceSpaceTypeStr) {
    XrReferenceSpaceCreateInfo reference_space_create_info = {XR_TYPE_REFERENCE_SPACE_CREATE_INFO};
    reference_space_create_info.poseInReferenceSpace = Math::Pose::Identity();

    if (EqualsIgnoreCase(referenceSpaceTypeStr, "View")) {
      reference_space_create_info.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_VIEW;
    }
    else if (EqualsIgnoreCase(referenceSpaceTypeStr, "ViewFront")) {
      // Render head-locked 2m in front of device.
      reference_space_create_info.poseInReferenceSpace = Math::Pose::Translation({0.f, 0.f, -2.f}),
      reference_space_create_info.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_VIEW;
    }
    else if (EqualsIgnoreCase(referenceSpaceTypeStr, "Local")) {
      reference_space_create_info.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_LOCAL;
    }
    else if (EqualsIgnoreCase(referenceSpaceTypeStr, "Stage")) {
      reference_space_create_info.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_STAGE;
    }
    else if (EqualsIgnoreCase(referenceSpaceTypeStr, "StageLeft")) {
      reference_space_create_info.poseInReferenceSpace =
          Math::Pose::RotateCCWAboutYAxis(0.f, {-2.f, 0.f, -2.f});
      reference_space_create_info.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_STAGE;
    }
    else if (EqualsIgnoreCase(referenceSpaceTypeStr, "StageRight")) {
      reference_space_create_info.poseInReferenceSpace =
          Math::Pose::RotateCCWAboutYAxis(0.f, {2.f, 0.f, -2.f});
      reference_space_create_info.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_STAGE;
    }
    else if (EqualsIgnoreCase(referenceSpaceTypeStr, "StageLeftRotated")) {
      reference_space_create_info.poseInReferenceSpace =
          Math::Pose::RotateCCWAboutYAxis(3.14f / 3.f, {-2.f, 0.5f, -2.f});
      reference_space_create_info.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_STAGE;
    }
    else if (EqualsIgnoreCase(referenceSpaceTypeStr, "StageRightRotated")) {
      reference_space_create_info.poseInReferenceSpace =
          Math::Pose::RotateCCWAboutYAxis(-3.14f / 3.f, {2.f, 0.5f, -2.f});
      reference_space_create_info.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_STAGE;
    }
    else {
      throw std::invalid_argument(
          Fmt("Unknown reference space type '%s'", referenceSpaceTypeStr.c_str()));
    }

    return reference_space_create_info;
  }
}