#pragma once

#include "Common/Challenge.h"

class CChallenge_20 : public CChallenge
{
private:
    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override { return CleanUp_Common(); }

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override { return CleanUp_Common(); }

    EErrorCode SetUp_Common(int const paddingSize);
    EErrorCode CleanUp_Common();

    int EnhanceImage();
    char GetEnhancedPixel(int const x, int const y) const;

    static std::string const sm_inputFilePath;
    std::string m_enhancementAlgorithm = "";
    char** m_imageFrontBuffer = nullptr;
    char** m_imageBackBuffer = nullptr;
    int m_allocatedImageWidth = 0;
    int m_allocatedImageHeight = 0;
    int m_imageWidth = 0;
    int m_imageHeight = 0;
    int m_imageAnchorX = 0;
    int m_imageAnchorY = 0;
};
