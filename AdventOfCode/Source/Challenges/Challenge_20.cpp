#include "Challenge_20.h"
#include "../Helpers/FileHelpers.h"

#define PART_ONE_ENHANCEMENT_COUNT 2
#define PART_TWO_ENHANCEMENT_COUNT 50

#define PIXEL_ON_CHAR '#'
#define PIXEL_OFF_CHAR '.'



////////////////////////////////////////////////////////////////////////////////////////////////////
std::string const CChallenge_20::sm_inputFilePath = "Inputs/Input_Challenge_20.txt";



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_20::SetUp_FirstPart()
{
    return SetUp_Common(PART_ONE_ENHANCEMENT_COUNT + 1);
}

EErrorCode CChallenge_20::Run_FirstPart()
{
    int onPixelCount = 0;
    for (int enhancementIdx = 0; enhancementIdx < PART_ONE_ENHANCEMENT_COUNT; ++enhancementIdx)
    {
        onPixelCount = EnhanceImage();
    }

    std::cout << "On pixels after " << PART_ONE_ENHANCEMENT_COUNT << " enhancements: " << onPixelCount << std::endl;
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_20::SetUp_SecondPart()
{
    return SetUp_Common(PART_TWO_ENHANCEMENT_COUNT + 1);
}

EErrorCode CChallenge_20::Run_SecondPart()
{
    int onPixelCount = 0;
    for (int enhancementIdx = 0; enhancementIdx < PART_TWO_ENHANCEMENT_COUNT; ++enhancementIdx)
    {
        onPixelCount = EnhanceImage();
    }

    std::cout << "On pixels after " << PART_TWO_ENHANCEMENT_COUNT << " enhancements: " << onPixelCount << std::endl;
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_20::SetUp_Common(int const paddingSize)
{
    std::vector<std::string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    m_enhancementAlgorithm = lines[0];

    m_imageAnchorX = paddingSize;
    m_imageAnchorY = paddingSize;

    m_imageWidth = static_cast<int>(lines[2].size());
    m_imageHeight = static_cast<int>(lines.size() - 2);

    m_allocatedImageWidth = m_imageWidth + paddingSize * 2;
    m_allocatedImageHeight = m_imageHeight + paddingSize * 2;

    m_imageFrontBuffer = new char*[m_allocatedImageHeight];
    m_imageBackBuffer = new char*[m_allocatedImageHeight];
    for (int bufferY = 0; bufferY < m_allocatedImageHeight; ++bufferY)
    {
        m_imageFrontBuffer[bufferY] = new char[m_allocatedImageWidth];
        m_imageBackBuffer[bufferY] = new char[m_allocatedImageWidth];
        for (int bufferX = 0; bufferX < m_allocatedImageWidth; ++bufferX)
        {
            m_imageFrontBuffer[bufferY][bufferX] = PIXEL_OFF_CHAR;
            m_imageBackBuffer[bufferY][bufferX] = PIXEL_OFF_CHAR;
        }
    }

    for (int bufferY = 0; bufferY < m_imageHeight; ++bufferY)
    {
        for (int bufferX = 0; bufferX < m_imageWidth; ++bufferX)
        {
            m_imageFrontBuffer[m_imageAnchorY + bufferY][m_imageAnchorX + bufferX] = lines[bufferY + 2][bufferX];
        }
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_20::CleanUp_Common()
{
    for (int bufferY = 0; bufferY < m_allocatedImageHeight; ++bufferY)
    {
        delete m_imageFrontBuffer[bufferY];
        delete m_imageBackBuffer[bufferY];
    }
    delete m_imageFrontBuffer;
    delete m_imageBackBuffer;

    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
int CChallenge_20::EnhanceImage()
{
    --m_imageAnchorX;
    --m_imageAnchorY;
    m_imageWidth += 2;
    m_imageHeight += 2;

    for (int bufferY = 0; bufferY < m_imageHeight; ++bufferY)
    {
        // TODO
        m_imageFrontBuffer[bufferY][m_imageAnchorX] = 0;
    }

    for (int bufferX = 0; bufferX < m_imageWidth; ++bufferX)
    {

    }

    int onPixelCount = 0;
    for (int bufferY = 0; bufferY < m_imageHeight; ++bufferY)
    {
        for (int bufferX = 0; bufferX < m_imageWidth; ++bufferX)
        {
            char const enhancedPixel = GetEnhancedPixel(bufferX, bufferY);
            m_imageBackBuffer[m_imageAnchorY + bufferY][m_imageAnchorX + bufferX] = enhancedPixel;
            if (enhancedPixel == PIXEL_ON_CHAR)
            {
                ++onPixelCount;
            }
        }
    }

    char** tempBuffer = m_imageBackBuffer;
    m_imageBackBuffer = m_imageFrontBuffer;
    m_imageFrontBuffer = tempBuffer;

    return onPixelCount;
}

char CChallenge_20::GetEnhancedPixel(int const x, int const y) const
{
    int algorithmIdx = 0;
    for (int yOffset = -1; yOffset <= 1; ++yOffset)
    {
        for (int xOffset = -1; xOffset <= 1; ++xOffset)
        {
            algorithmIdx = algorithmIdx << 1;
            algorithmIdx += m_imageFrontBuffer[m_imageAnchorY + y + yOffset][m_imageAnchorX + x + xOffset] == PIXEL_ON_CHAR ? 1 : 0;
        }
    }

    return m_enhancementAlgorithm[algorithmIdx];
}
